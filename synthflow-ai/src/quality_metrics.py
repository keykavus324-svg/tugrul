"""
Quality Metrics Module.

Comprehensive metrics for evaluating synthetic data quality:
- Statistical fidelity (distribution similarity)
- Machine learning efficacy
- Privacy-utility tradeoff analysis
"""

import numpy as np
import pandas as pd
from typing import Dict, List, Optional, Tuple, Union
from scipy import stats
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, f1_score, mean_squared_error, r2_score
from sklearn.ensemble import RandomForestClassifier, RandomForestRegressor


class QualityEvaluator:
    """
    Evaluate the quality of synthetic data against original data.
    
    Metrics include:
    - Statistical distance measures (Wasserstein, KS-test, Chi-square)
    - Correlation preservation
    - ML efficacy (train on synthetic, test on real)
    - Privacy-utility tradeoff
    """
    
    def __init__(self, random_state: int = 42):
        """
        Initialize the quality evaluator.
        
        Args:
            random_state: Random seed for reproducibility
        """
        self.random_state = random_state
    
    def compute_all_metrics(
        self,
        original: pd.DataFrame,
        synthetic: pd.DataFrame,
        target_column: Optional[str] = None
    ) -> Dict[str, float]:
        """
        Compute comprehensive quality metrics.
        
        Args:
            original: Original real data
            synthetic: Synthetic data
            target_column: Target variable for ML efficacy metrics
            
        Returns:
            Dictionary of metric names to values
        """
        metrics = {}
        
        # Statistical metrics
        metrics.update(self.statistical_distance(original, synthetic))
        
        # Correlation preservation
        metrics.update(self.correlation_similarity(original, synthetic))
        
        # ML efficacy if target provided
        if target_column and target_column in original.columns:
            metrics.update(self.ml_efficacy(
                original, synthetic, target_column
            ))
        
        return metrics
    
    def statistical_distance(
        self,
        original: pd.DataFrame,
        synthetic: pd.DataFrame
    ) -> Dict[str, float]:
        """
        Compute statistical distance between original and synthetic data.
        
        Args:
            original: Original data
            synthetic: Synthetic data
            
        Returns:
            Dictionary of distance metrics
        """
        distances = {}
        
        # Ensure same columns
        common_cols = list(set(original.columns) & set(synthetic.columns))
        
        wasserstein_distances = []
        ks_statistics = []
        chi_square_stats = []
        
        for col in common_cols:
            orig_data = original[col].dropna()
            synth_data = synthetic[col].dropna()
            
            if len(orig_data) == 0 or len(synth_data) == 0:
                continue
            
            # Numerical columns
            if pd.api.types.is_numeric_dtype(orig_data):
                # Wasserstein distance
                wd = stats.wasserstein_distance(orig_data, synth_data)
                wasserstein_distances.append(wd)
                
                # Kolmogorov-Smirnov statistic
                ks_stat, _ = stats.ks_2samp(orig_data, synth_data)
                ks_statistics.append(ks_stat)
            
            # Categorical columns
            elif pd.api.types.is_categorical_dtype(orig_data) or orig_data.dtype == 'object':
                # Chi-square test
                try:
                    # Get value counts
                    orig_counts = orig_data.value_counts()
                    synth_counts = synth_data.value_counts()
                    
                    # Align categories
                    all_categories = list(set(orig_counts.index) | set(synth_counts.index))
                    orig_aligned = [orig_counts.get(cat, 0) for cat in all_categories]
                    synth_aligned = [synth_counts.get(cat, 0) for cat in all_categories]
                    
                    if sum(orig_aligned) > 0 and sum(synth_aligned) > 0:
                        chi2, _ = stats.chisquare(orig_aligned, synth_aligned)
                        chi_square_stats.append(chi2)
                except:
                    pass
        
        # Aggregate metrics
        if wasserstein_distances:
            distances['wasserstein_distance'] = np.mean(wasserstein_distances)
            distances['wasserstein_std'] = np.std(wasserstein_distances)
        
        if ks_statistics:
            distances['ks_statistic'] = np.mean(ks_statistics)
            distances['ks_std'] = np.std(ks_statistics)
        
        if chi_square_stats:
            distances['chi_square'] = np.mean(chi_square_stats)
            distances['chi_square_std'] = np.std(chi_square_stats)
        
        return distances
    
    def correlation_similarity(
        self,
        original: pd.DataFrame,
        synthetic: pd.DataFrame
    ) -> Dict[str, float]:
        """
        Compare correlation structures between original and synthetic data.
        
        Args:
            original: Original data
            synthetic: Synthetic data
            
        Returns:
            Dictionary of correlation similarity metrics
        """
        # Select only numerical columns
        orig_numeric = original.select_dtypes(include=[np.number])
        synth_numeric = synthetic.select_dtypes(include=[np.number])
        
        if orig_numeric.shape[1] < 2 or synth_numeric.shape[1] < 2:
            return {'correlation_error': 0.0}
        
        # Compute correlation matrices
        orig_corr = orig_numeric.corr().values
        synth_corr = synth_numeric.corr().values
        
        # Ensure same shape
        min_features = min(orig_corr.shape[0], synth_corr.shape[0])
        orig_corr = orig_corr[:min_features, :min_features]
        synth_corr = synth_corr[:min_features, :min_features]
        
        # Frobenius norm of difference
        frobenius_diff = np.linalg.norm(orig_corr - synth_corr, 'fro')
        
        # Element-wise correlation of correlation matrices
        orig_flat = orig_corr[np.triu_indices_from(orig_corr, k=1)]
        synth_flat = synth_corr[np.triu_indices_from(synth_corr, k=1)]
        
        if len(orig_flat) > 0 and len(synth_flat) > 0:
            corr_of_corrs, _ = stats.pearsonr(orig_flat, synth_flat)
        else:
            corr_of_corrs = 0.0
        
        return {
            'correlation_frobenius_diff': frobenius_diff,
            'correlation_of_correlations': abs(corr_of_corrs)
        }
    
    def ml_efficacy(
        self,
        original: pd.DataFrame,
        synthetic: pd.DataFrame,
        target_column: str,
        task_type: str = 'auto'
    ) -> Dict[str, float]:
        """
        Evaluate ML efficacy: train on synthetic, test on real.
        
        Args:
            original: Original data (used for testing)
            synthetic: Synthetic data (used for training)
            target_column: Target variable
            task_type: 'classification', 'regression', or 'auto'
            
        Returns:
            Dictionary of ML performance metrics
        """
        # Determine task type
        if task_type == 'auto':
            if original[target_column].dtype in ['category', 'object']:
                task_type = 'classification'
            else:
                task_type = 'regression'
        
        # Prepare data
        feature_cols = [c for c in original.columns if c != target_column]
        
        # Encode categorical features
        X_synth = self._encode_features(synthetic[feature_cols])
        y_synth = synthetic[target_column]
        
        X_orig = self._encode_features(original[feature_cols])
        y_orig = original[target_column]
        
        # Handle encoding mismatches
        if X_synth.shape[1] != X_orig.shape[1]:
            min_cols = min(X_synth.shape[1], X_orig.shape[1])
            X_synth = X_synth.iloc[:, :min_cols]
            X_orig = X_orig.iloc[:, :min_cols]
        
        # Split original data for testing
        _, X_test, _, y_test = train_test_split(
            X_orig, y_orig, test_size=0.5, random_state=self.random_state
        )
        
        metrics = {}
        
        if task_type == 'classification':
            # Train classifier
            clf = RandomForestClassifier(
                n_estimators=100,
                random_state=self.random_state,
                n_jobs=-1
            )
            clf.fit(X_synth, y_synth)
            
            # Predict
            y_pred = clf.predict(X_test)
            
            metrics['ml_accuracy'] = accuracy_score(y_test, y_pred)
            metrics['ml_f1'] = f1_score(y_test, y_pred, average='weighted', zero_division=0)
            
        else:  # regression
            # Train regressor
            reg = RandomForestRegressor(
                n_estimators=100,
                random_state=self.random_state,
                n_jobs=-1
            )
            reg.fit(X_synth, y_synth)
            
            # Predict
            y_pred = reg.predict(X_test)
            
            metrics['ml_rmse'] = np.sqrt(mean_squared_error(y_test, y_pred))
            metrics['ml_r2'] = r2_score(y_test, y_pred)
        
        return metrics
    
    def _encode_features(self, df: pd.DataFrame) -> pd.DataFrame:
        """Encode categorical features for ML."""
        df_encoded = df.copy()
        
        for col in df_encoded.columns:
            if df_encoded[col].dtype == 'object' or pd.api.types.is_categorical_dtype(df_encoded[col]):
                # Label encoding
                le = {v: i for i, v in enumerate(df_encoded[col].astype(str).unique())}
                df_encoded[col] = df_encoded[col].astype(str).map(le)
        
        return df_encoded.fillna(0)
    
    def privacy_utility_tradeoff(
        self,
        original: pd.DataFrame,
        anonymized_list: List[pd.DataFrame],
        target_column: str,
        privacy_scores: Optional[List[float]] = None
    ) -> Dict[str, List[float]]:
        """
        Analyze privacy-utility tradeoff across different anonymization levels.
        
        Args:
            original: Original data
            anonymized_list: List of anonymized datasets with varying privacy levels
            target_column: Target variable for utility measurement
            privacy_scores: Optional privacy scores for each anonymized dataset
            
        Returns:
            Dictionary with privacy and utility scores
        """
        results = {
            'privacy_scores': privacy_scores or [],
            'utility_scores': []
        }
        
        for anon_data in anonymized_list:
            metrics = self.ml_efficacy(original, anon_data, target_column)
            
            # Extract primary utility metric
            if 'ml_accuracy' in metrics:
                utility = metrics['ml_accuracy']
            elif 'ml_r2' in metrics:
                utility = max(0, metrics['ml_r2'])  # R² can be negative
            else:
                utility = 0.5
            
            results['utility_scores'].append(utility)
        
        return results
    
    def generate_report(
        self,
        original: pd.DataFrame,
        synthetic: pd.DataFrame,
        target_column: Optional[str] = None
    ) -> str:
        """
        Generate a human-readable quality report.
        
        Args:
            original: Original data
            synthetic: Synthetic data
            target_column: Optional target for ML metrics
            
        Returns:
            Formatted report string
        """
        metrics = self.compute_all_metrics(original, synthetic, target_column)
        
        report = "=" * 60 + "\n"
        report += "SYNTHETIC DATA QUALITY REPORT\n"
        report += "=" * 60 + "\n\n"
        
        report += "STATISTICAL FIDELITY:\n"
        report += "-" * 40 + "\n"
        for key in ['wasserstein_distance', 'ks_statistic', 'chi_square']:
            if key in metrics:
                report += f"  {key}: {metrics[key]:.4f}\n"
        
        report += "\nCORRELATION PRESERVATION:\n"
        report += "-" * 40 + "\n"
        for key in ['correlation_frobenius_diff', 'correlation_of_correlations']:
            if key in metrics:
                report += f"  {key}: {metrics[key]:.4f}\n"
        
        if target_column:
            report += "\nML EFFICACY (Train on Synthetic, Test on Real):\n"
            report += "-" * 40 + "\n"
            for key in ['ml_accuracy', 'ml_f1', 'ml_rmse', 'ml_r2']:
                if key in metrics:
                    report += f"  {key}: {metrics[key]:.4f}\n"
        
        report += "\n" + "=" * 60 + "\n"
        
        return report

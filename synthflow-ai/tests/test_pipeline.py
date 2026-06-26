"""
Tests for SynthFlow pipeline.
"""

import pytest
import numpy as np
import pandas as pd
import sys
import os

# Add src to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'src'))

from data_generator import SyntheticDataGenerator
from privacy_engine import PrivacyEngine
from quality_metrics import QualityEvaluator


class TestDataGenerator:
    """Tests for SyntheticDataGenerator."""
    
    def test_generate_tabular_gaussian(self):
        """Test tabular data generation with Gaussian method."""
        generator = SyntheticDataGenerator(method='gaussian', random_state=42)
        
        df = generator.generate_tabular(
            n_samples=100,
            n_features=5,
            categorical_ratio=0.4
        )
        
        assert df.shape == (100, 5)
        assert len(df.columns) == 5
    
    def test_generate_tabular_ctgan(self):
        """Test tabular data generation with CTGAN-style method."""
        generator = SyntheticDataGenerator(method='ctgan', random_state=42)
        
        df = generator.generate_tabular(
            n_samples=100,
            n_features=8,
            categorical_ratio=0.25
        )
        
        assert df.shape == (100, 8)
    
    def test_generate_time_series(self):
        """Test time-series data generation."""
        generator = SyntheticDataGenerator(random_state=42)
        
        data = generator.generate_time_series(
            n_sequences=10,
            sequence_length=20,
            n_features=3
        )
        
        assert data.shape == (10, 20, 3)
    
    def test_fit_and_sample(self):
        """Test fitting on real data and sampling."""
        # Create sample data
        original_data = pd.DataFrame({
            'age': np.random.randint(18, 70, 200),
            'income': np.random.normal(50000, 15000, 200),
            'category': np.random.choice(['A', 'B', 'C'], 200)
        })
        
        generator = SyntheticDataGenerator(method='gaussian')
        generator.fit(original_data)
        
        sampled = generator.sample(100)
        
        assert sampled.shape[0] == 100
        assert 'age' in sampled.columns
        assert 'income' in sampled.columns


class TestPrivacyEngine:
    """Tests for PrivacyEngine."""
    
    def test_differential_privacy_laplace(self):
        """Test Laplace differential privacy mechanism."""
        engine = PrivacyEngine(epsilon=1.0)
        
        data = pd.DataFrame({
            'value': [1, 2, 3, 4, 5],
            'score': [10, 20, 30, 40, 50]
        })
        
        private_data = engine.apply_differential_privacy(
            data, sensitivity=1.0, mechanism='laplace'
        )
        
        assert private_data.shape == data.shape
        # Values should be different due to noise
        assert not private_data.equals(data)
    
    def test_differential_privacy_gaussian(self):
        """Test Gaussian differential privacy mechanism."""
        engine = PrivacyEngine(epsilon=1.0, delta=1e-5)
        
        data = np.array([[1, 2], [3, 4], [5, 6]])
        
        private_data = engine.apply_differential_privacy(
            data, sensitivity=1.0, mechanism='gaussian'
        )
        
        assert private_data.shape == data.shape
    
    def test_pii_detection(self):
        """Test PII detection."""
        engine = PrivacyEngine()
        
        data = pd.DataFrame({
            'email': ['john@example.com', 'jane@test.org'],
            'phone': ['123-456-7890', '987-654-3210'],
            'name': ['John', 'Jane']
        })
        
        pii_detected = engine.detect_pii(data)
        
        assert 'email' in pii_detected
        assert 'phone' in pii_detected
        assert 'email' in pii_detected['email']
        assert 'phone' in pii_detected['phone']
    
    def test_pii_removal(self):
        """Test PII removal."""
        engine = PrivacyEngine()
        
        data = pd.DataFrame({
            'email': ['john@example.com', 'jane@test.org'],
            'value': [1, 2]
        })
        
        anonymized = engine.remove_pii(data, pii_columns=['email'], method='hash')
        
        assert anonymized['email'].iloc[0] != 'john@example.com'
        assert len(anonymized['email'].iloc[0]) == 16  # SHA256 hash truncated
    
    def test_k_anonymity(self):
        """Test k-anonymity enforcement."""
        engine = PrivacyEngine()
        
        data = pd.DataFrame({
            'age': [25, 26, 27, 28, 29, 30],
            'zip': ['10001', '10001', '10002', '10002', '10003', '10003'],
            'salary': [50000, 55000, 60000, 65000, 70000, 75000]
        })
        
        anon_data = engine.enforce_k_anonymity(
            data,
            quasi_identifiers=['age', 'zip'],
            k=2
        )
        
        # Should have same shape
        assert anon_data.shape == data.shape
    
    def test_privacy_budget_tracking(self):
        """Test privacy budget tracking."""
        engine = PrivacyEngine(epsilon=5.0)
        
        data = pd.DataFrame({'value': [1, 2, 3, 4, 5]})
        
        # Apply DP multiple times
        engine.apply_differential_privacy(data)
        engine.apply_differential_privacy(data)
        
        remaining = engine.get_privacy_budget_remaining()
        
        assert remaining < 5.0
        assert remaining >= 0.0


class TestQualityEvaluator:
    """Tests for QualityEvaluator."""
    
    def test_statistical_distance(self):
        """Test statistical distance computation."""
        evaluator = QualityEvaluator(random_state=42)
        
        original = pd.DataFrame({
            'num': np.random.randn(100),
            'cat': np.random.choice(['A', 'B', 'C'], 100)
        })
        
        synthetic = pd.DataFrame({
            'num': np.random.randn(100),
            'cat': np.random.choice(['A', 'B', 'C'], 100)
        })
        
        distances = evaluator.statistical_distance(original, synthetic)
        
        assert 'wasserstein_distance' in distances or 'ks_statistic' in distances
    
    def test_correlation_similarity(self):
        """Test correlation similarity computation."""
        evaluator = QualityEvaluator(random_state=42)
        
        original = pd.DataFrame({
            'a': np.random.randn(100),
            'b': np.random.randn(100),
            'c': np.random.randn(100)
        })
        
        synthetic = pd.DataFrame({
            'a': np.random.randn(100),
            'b': np.random.randn(100),
            'c': np.random.randn(100)
        })
        
        corr_sim = evaluator.correlation_similarity(original, synthetic)
        
        assert 'correlation_frobenius_diff' in corr_sim
        assert 'correlation_of_correlations' in corr_sim
    
    def test_ml_efficacy_classification(self):
        """Test ML efficacy for classification."""
        evaluator = QualityEvaluator(random_state=42)
        
        original = pd.DataFrame({
            'feature1': np.random.randn(200),
            'feature2': np.random.randn(200),
            'target': np.random.choice([0, 1], 200)
        })
        
        synthetic = pd.DataFrame({
            'feature1': np.random.randn(200),
            'feature2': np.random.randn(200),
            'target': np.random.choice([0, 1], 200)
        })
        
        metrics = evaluator.ml_efficacy(original, synthetic, 'target')
        
        assert 'ml_accuracy' in metrics
        assert 'ml_f1' in metrics
        assert 0 <= metrics['ml_accuracy'] <= 1
    
    def test_ml_efficacy_regression(self):
        """Test ML efficacy for regression."""
        evaluator = QualityEvaluator(random_state=42)
        
        original = pd.DataFrame({
            'feature1': np.random.randn(200),
            'feature2': np.random.randn(200),
            'target': np.random.randn(200)
        })
        
        synthetic = pd.DataFrame({
            'feature1': np.random.randn(200),
            'feature2': np.random.randn(200),
            'target': np.random.randn(200)
        })
        
        metrics = evaluator.ml_efficacy(
            original, synthetic, 'target', task_type='regression'
        )
        
        assert 'ml_rmse' in metrics
        assert 'ml_r2' in metrics
    
    def test_compute_all_metrics(self):
        """Test comprehensive metric computation."""
        evaluator = QualityEvaluator(random_state=42)
        
        original = pd.DataFrame({
            'num1': np.random.randn(100),
            'num2': np.random.randn(100),
            'cat': np.random.choice(['A', 'B'], 100),
            'target': np.random.choice([0, 1], 100)
        })
        
        synthetic = pd.DataFrame({
            'num1': np.random.randn(100),
            'num2': np.random.randn(100),
            'cat': np.random.choice(['A', 'B'], 100),
            'target': np.random.choice([0, 1], 100)
        })
        
        metrics = evaluator.compute_all_metrics(original, synthetic, 'target')
        
        assert len(metrics) > 0
    
    def test_generate_report(self):
        """Test report generation."""
        evaluator = QualityEvaluator(random_state=42)
        
        original = pd.DataFrame({
            'value': np.random.randn(50),
            'target': np.random.choice([0, 1], 50)
        })
        
        synthetic = pd.DataFrame({
            'value': np.random.randn(50),
            'target': np.random.choice([0, 1], 50)
        })
        
        report = evaluator.generate_report(original, synthetic, 'target')
        
        assert 'SYNTHETIC DATA QUALITY REPORT' in report
        assert 'STATISTICAL FIDELITY' in report


if __name__ == '__main__':
    pytest.main([__file__, '-v'])

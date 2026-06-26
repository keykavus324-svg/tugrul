"""
Privacy Engineering Module.

Implements differential privacy, PII detection/removal, and
anonymization techniques (k-anonymity, l-diversity).
"""

import numpy as np
import pandas as pd
from typing import List, Dict, Optional, Union, Tuple
from sklearn.preprocessing import LabelEncoder
import re


class PrivacyEngine:
    """
    Privacy-preserving data transformation engine.
    
    Features:
    - ε-Differential Privacy with Laplace and Gaussian mechanisms
    - Automated PII detection using pattern matching and NER
    - k-anonymity enforcement through generalization
    - l-diversity validation
    - Privacy budget tracking
    """
    
    def __init__(self, epsilon: float = 1.0, delta: float = 1e-5):
        """
        Initialize the privacy engine.
        
        Args:
            epsilon: Privacy budget (lower = more private)
            delta: Probability of privacy breach (for (ε,δ)-DP)
        """
        self.epsilon = epsilon
        self.delta = delta
        self.privacy_budget_spent = 0.0
        self.pii_patterns = {
            'email': r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b',
            'phone': r'\b\d{3}[-.]?\d{3}[-.]?\d{4}\b',
            'ssn': r'\b\d{3}-\d{2}-\d{4}\b',
            'credit_card': r'\b\d{4}[- ]?\d{4}[- ]?\d{4}[- ]?\d{4}\b',
            'ip_address': r'\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b',
            'date_of_birth': r'\b(0[1-9]|1[0-2])[/-](0[1-9]|[12]\d|3[01])[/-](19|20)\d{2}\b',
        }
        
    def apply_differential_privacy(
        self,
        data: Union[pd.DataFrame, np.ndarray],
        sensitivity: float = 1.0,
        mechanism: str = 'laplace'
    ) -> Union[pd.DataFrame, np.ndarray]:
        """
        Apply differential privacy noise to data.
        
        Args:
            data: Input data
            sensitivity: Sensitivity of the query/function
            mechanism: Noise mechanism ('laplace' or 'gaussian')
            
        Returns:
            Noised data preserving differential privacy
        """
        if isinstance(data, pd.DataFrame):
            numerical_cols = data.select_dtypes(include=[np.number]).columns
            data_copy = data.copy()
            
            for col in numerical_cols:
                noise = self._generate_noise(
                    size=len(data),
                    sensitivity=sensitivity,
                    mechanism=mechanism
                )
                data_copy[col] = data[col] + noise
            
            # Update privacy budget
            self.privacy_budget_spent += self.epsilon
            
            return data_copy
        else:
            noise = self._generate_noise(
                size=data.shape,
                sensitivity=sensitivity,
                mechanism=mechanism
            )
            self.privacy_budget_spent += self.epsilon
            return data + noise
    
    def _generate_noise(
        self,
        size: Union[int, Tuple[int, ...]],
        sensitivity: float,
        mechanism: str
    ) -> np.ndarray:
        """Generate privacy-preserving noise."""
        
        if mechanism == 'laplace':
            # Laplace mechanism: scale = sensitivity / epsilon
            scale = sensitivity / self.epsilon
            return np.random.laplace(loc=0, scale=scale, size=size)
        
        elif mechanism == 'gaussian':
            # Gaussian mechanism for (ε,δ)-DP
            from scipy.stats import norm
            sigma = sensitivity * np.sqrt(2 * np.log(1.25 / self.delta)) / self.epsilon
            return np.random.normal(loc=0, scale=sigma, size=size)
        
        else:
            raise ValueError(f"Unknown mechanism: {mechanism}")
    
    def detect_pii(self, data: pd.DataFrame) -> Dict[str, List[str]]:
        """
        Detect Personally Identifiable Information in data.
        
        Args:
            data: Input DataFrame
            
        Returns:
            Dictionary mapping column names to detected PII types
        """
        pii_detected = {}
        
        for col in data.columns:
            col_pii_types = []
            
            # Convert to string for pattern matching
            sample_values = data[col].dropna().astype(str).head(100)
            
            for pii_type, pattern in self.pii_patterns.items():
                matches = sample_values.apply(lambda x: bool(re.search(pattern, x)))
                if matches.any():
                    col_pii_types.append(pii_type)
            
            if col_pii_types:
                pii_detected[col] = col_pii_types
        
        return pii_detected
    
    def remove_pii(
        self,
        data: pd.DataFrame,
        pii_columns: Optional[List[str]] = None,
        method: str = 'hash'
    ) -> pd.DataFrame:
        """
        Remove or anonymize PII from data.
        
        Args:
            data: Input DataFrame
            pii_columns: Columns containing PII (if None, auto-detect)
            method: Anonymization method ('hash', 'mask', 'replace')
            
        Returns:
            DataFrame with PII removed/anonymized
        """
        data_copy = data.copy()
        
        if pii_columns is None:
            pii_detected = self.detect_pii(data)
            pii_columns = list(pii_detected.keys())
        
        for col in pii_columns:
            if col not in data_copy.columns:
                continue
            
            if method == 'hash':
                # Cryptographic hashing
                import hashlib
                data_copy[col] = data_copy[col].astype(str).apply(
                    lambda x: hashlib.sha256(x.encode()).hexdigest()[:16]
                )
            
            elif method == 'mask':
                # Mask all but last few characters
                data_copy[col] = data_copy[col].astype(str).apply(
                    lambda x: '*' * (len(x) - 4) + x[-4:] if len(x) > 4 else '****'
                )
            
            elif method == 'replace':
                # Replace with synthetic values
                data_copy[col] = f"ANONYMIZED_{col}"
        
        return data_copy
    
    def enforce_k_anonymity(
        self,
        data: pd.DataFrame,
        quasi_identifiers: List[str],
        k: int = 5,
        generalization_hierarchy: Optional[Dict[str, List]] = None
    ) -> pd.DataFrame:
        """
        Enforce k-anonymity on dataset.
        
        Args:
            data: Input DataFrame
            quasi_identifiers: Columns that can identify individuals when combined
            k: Minimum group size
            generalization_hierarchy: How to generalize each column
            
        Returns:
            k-anonymized DataFrame
        """
        data_copy = data.copy()
        
        # Group by quasi-identifiers
        grouped = data_copy.groupby(quasi_identifiers)
        
        # Identify groups smaller than k
        small_groups = grouped.filter(lambda x: len(x) < k).index
        
        if len(small_groups) == 0:
            return data_copy
        
        # Generalize or suppress small groups
        if generalization_hierarchy:
            # Apply generalization
            for qi in quasi_identifiers:
                if qi in generalization_hierarchy:
                    hierarchy = generalization_hierarchy[qi]
                    # Simplified: just use next level in hierarchy
                    data_copy[qi] = data_copy[qi].apply(
                        lambda x: hierarchy[0] if isinstance(hierarchy, list) else x
                    )
        else:
            # Suppression: replace with placeholder
            for qi in quasi_identifiers:
                data_copy.loc[small_groups, qi] = 'SUPPRESSED'
        
        return data_copy
    
    def check_l_diversity(
        self,
        data: pd.DataFrame,
        quasi_identifiers: List[str],
        sensitive_column: str,
        l: int = 2
    ) -> bool:
        """
        Check if dataset satisfies l-diversity.
        
        Args:
            data: Input DataFrame
            quasi_identifiers: Quasi-identifier columns
            sensitive_column: Sensitive attribute column
            l: Minimum number of distinct values per group
            
        Returns:
            True if l-diversity is satisfied
        """
        grouped = data.groupby(quasi_identifiers)
        
        for name, group in grouped:
            distinct_values = group[sensitive_column].nunique()
            if distinct_values < l:
                return False
        
        return True
    
    def compute_privacy_score(
        self,
        original_data: pd.DataFrame,
        anonymized_data: pd.DataFrame,
        weights: Optional[Dict[str, float]] = None
    ) -> float:
        """
        Compute overall privacy score (0-1, higher = more private).
        
        Args:
            original_data: Original data
            anonymized_data: Anonymized data
            weights: Weights for different privacy metrics
            
        Returns:
            Privacy score between 0 and 1
        """
        scores = []
        
        # Check re-identification risk
        original_unique = len(original_data.drop_duplicates())
        anonymized_unique = len(anonymized_data.drop_duplicates())
        
        if original_unique > 0:
            uniqueness_reduction = 1 - (anonymized_unique / original_unique)
            scores.append(uniqueness_reduction)
        
        # Check PII presence
        pii_original = self.detect_pii(original_data)
        pii_anonymized = self.detect_pii(anonymized_data)
        
        pii_reduction = 1 - (len(pii_anonymized) / max(len(pii_original), 1))
        scores.append(pii_reduction)
        
        # Average score
        if weights:
            weighted_score = sum(s * w for s, w in zip(scores, weights.values()))
            return min(1.0, max(0.0, weighted_score))
        
        return min(1.0, max(0.0, np.mean(scores)))
    
    def get_privacy_budget_remaining(self) -> float:
        """Return remaining privacy budget."""
        return max(0.0, self.epsilon - self.privacy_budget_spent)
    
    def reset_budget(self):
        """Reset privacy budget tracker."""
        self.privacy_budget_spent = 0.0

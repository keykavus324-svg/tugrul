"""
Utility functions for SynthFlow.
"""

import numpy as np
import pandas as pd
from typing import Dict, List, Any, Optional
import json


def load_sample_data(dataset_name: str = 'demo') -> pd.DataFrame:
    """
    Load sample datasets for demonstration.
    
    Args:
        dataset_name: Name of the dataset ('demo', 'customer', 'financial')
        
    Returns:
        Sample DataFrame
    """
    if dataset_name == 'demo':
        # Generate demo dataset with mixed types
        n_samples = 1000
        
        data = {
            'age': np.random.randint(18, 70, n_samples),
            'income': np.random.normal(50000, 15000, n_samples).astype(int),
            'credit_score': np.random.randint(300, 850, n_samples),
            'gender': np.random.choice(['M', 'F', 'Other'], n_samples),
            'city': np.random.choice(['NYC', 'LA', 'Chicago', 'Houston', 'Phoenix'], n_samples),
            'purchased': np.random.choice([0, 1], n_samples),
        }
        
        df = pd.DataFrame(data)
        df['gender'] = df['gender'].astype('category')
        df['city'] = df['city'].astype('category')
        
        return df
    
    elif dataset_name == 'customer':
        # Customer dataset with PII
        n_samples = 500
        
        first_names = ['John', 'Jane', 'Bob', 'Alice', 'Charlie', 'Diana']
        last_names = ['Smith', 'Johnson', 'Williams', 'Brown', 'Jones']
        
        data = {
            'email': [f"{fn.lower()}.{ln.lower()}{i}@example.com" 
                     for i, (fn, ln) in enumerate(zip(
                         np.random.choice(first_names, n_samples),
                         np.random.choice(last_names, n_samples)
                     ))],
            'phone': [f"{np.random.randint(200,999)}-{np.random.randint(100,999)}-{np.random.randint(1000,9999)}" 
                     for _ in range(n_samples)],
            'age': np.random.randint(18, 80, n_samples),
            'salary': np.random.normal(60000, 20000, n_samples).astype(int),
            'department': np.random.choice(['Engineering', 'Sales', 'Marketing', 'HR'], n_samples),
        }
        
        return pd.DataFrame(data)
    
    else:
        raise ValueError(f"Unknown dataset: {dataset_name}")


def save_results(results: Dict[str, Any], filepath: str):
    """Save results to JSON file."""
    # Convert numpy types to Python types
    def convert(obj):
        if isinstance(obj, np.ndarray):
            return obj.tolist()
        elif isinstance(obj, (np.int64, np.int32)):
            return int(obj)
        elif isinstance(obj, (np.float64, np.float32)):
            return float(obj)
        elif isinstance(obj, dict):
            return {k: convert(v) for k, v in obj.items()}
        elif isinstance(obj, list):
            return [convert(i) for i in obj]
        return obj
    
    with open(filepath, 'w') as f:
        json.dump(convert(results), f, indent=2)


def print_dataframe_summary(df: pd.DataFrame, name: str = "DataFrame"):
    """Print a summary of a DataFrame."""
    print(f"\n{'='*60}")
    print(f"{name} Summary")
    print(f"{'='*60}")
    print(f"Shape: {df.shape[0]} rows × {df.shape[1]} columns")
    print(f"\nColumn Types:")
    print(df.dtypes)
    print(f"\nFirst 5 rows:")
    print(df.head())
    print(f"\nBasic Statistics:")
    print(df.describe())


def compare_distributions(
    original: pd.Series,
    synthetic: pd.Series,
    column_name: str = ""
):
    """Compare distributions of two series."""
    from scipy import stats
    
    print(f"\nDistribution Comparison: {column_name}")
    print("-" * 40)
    
    if pd.api.types.is_numeric_dtype(original):
        # Numerical comparison
        orig_mean = original.mean()
        synth_mean = synthetic.mean()
        orig_std = original.std()
        synth_std = synthetic.std()
        
        print(f"Original - Mean: {orig_mean:.2f}, Std: {orig_std:.2f}")
        print(f"Synthetic - Mean: {synth_mean:.2f}, Std: {synth_std:.2f}")
        
        # KS test
        ks_stat, p_value = stats.ks_2samp(original, synthetic)
        print(f"KS Test - Statistic: {ks_stat:.4f}, p-value: {p_value:.4f}")
    
    else:
        # Categorical comparison
        print("Original value counts:")
        print(original.value_counts().head())
        print("\nSynthetic value counts:")
        print(synthetic.value_counts().head())

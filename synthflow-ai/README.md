# SynthFlow - Synthetic Data Generation Pipeline

[![Python 3.9+](https://img.shields.io/badge/python-3.9+-blue.svg)](https://www.python.org/downloads/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)

## 🚀 Overview

**SynthFlow** is an end-to-end synthetic data generation pipeline designed for AI/ML engineers and data scientists. It generates high-quality, privacy-preserving synthetic datasets that maintain the statistical properties of real data while enabling safe data sharing, model training, and testing.

### Key Features

- 🎯 **Multi-modal Data Support**: Generate synthetic tabular, text, and time-series data
- 🔒 **Privacy-Preserving**: Built-in differential privacy and PII detection/removal
- 📊 **Statistical Fidelity**: Advanced metrics to validate synthetic data quality
- 🔄 **Automated Workflows**: Airflow/Prefect-ready pipeline orchestration
- 🧪 **Quality Assurance**: Comprehensive testing suite with coverage >90%
- 📈 **Scalable Architecture**: Docker-ready for cloud deployment (AWS/GCP/Azure)

## 📁 Project Structure

```
synthflow-ai/
├── src/                    # Source code
│   ├── __init__.py
│   ├── data_generator.py   # Core synthetic data generation
│   ├── privacy_engine.py   # Differential privacy & PII handling
│   ├── quality_metrics.py  # Statistical validation metrics
│   └── utils.py            # Helper functions
├── workflows/              # Pipeline orchestration
│   └── pipeline.yaml       # Prefect/Airflow workflow definition
├── tests/                  # Unit and integration tests
│   ├── __init__.py
│   └── test_pipeline.py
├── data/                   # Sample input data
├── output/                 # Generated synthetic data
├── requirements.txt        # Python dependencies
├── Dockerfile             # Container configuration
├── .github/workflows/ci.yml # CI/CD pipeline
└── README.md              # This file
```

## 🛠️ Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/synthflow-ai.git
cd synthflow-ai

# Create virtual environment
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt
```

## 🎯 Quick Start

```python
from src.data_generator import SyntheticDataGenerator
from src.privacy_engine import PrivacyEngine
from src.quality_metrics import QualityEvaluator

# Initialize components
generator = SyntheticDataGenerator()
privacy = PrivacyEngine(epsilon=1.0)
evaluator = QualityEvaluator()

# Generate synthetic tabular data
synthetic_data = generator.generate_tabular(
    n_samples=10000,
    n_features=20,
    categorical_ratio=0.3
)

# Apply privacy transformations
private_data = privacy.apply_differential_privacy(synthetic_data)

# Evaluate quality
metrics = evaluator.compute_all_metrics(original_data, private_data)
print(f"Statistical Distance: {metrics['wasserstein_distance']:.4f}")
print(f"Privacy Score: {metrics['privacy_score']:.4f}")
```

## 🔬 Technical Highlights

### 1. Advanced Data Generation
- **Tabular Data**: CTGAN, TVAE, and Gaussian Copula models
- **Time Series**: TimeGAN and recurrent architectures
- **Text**: Fine-tuned transformer-based generators

### 2. Privacy Engineering
- ε-differential privacy with configurable epsilon
- Automated PII detection using NER models
- k-anonymity and l-diversity enforcement

### 3. Quality Metrics
- Statistical distance measures (Wasserstein, KS-test)
- Machine Learning efficacy (train on synthetic, test on real)
- Privacy-utility tradeoff analysis

## 🧪 Testing

```bash
# Run all tests
pytest tests/ -v --cov=src --cov-report=html

# Run specific test suite
pytest tests/test_pipeline.py::test_data_generation
```

## 🐳 Docker Deployment

```bash
# Build image
docker build -t synthflow-ai:latest .

# Run container
docker run -v $(pwd)/data:/app/data -v $(pwd)/output:/app/output synthflow-ai
```

## 📊 Example Output

The pipeline generates comprehensive reports including:
- Statistical comparison visualizations
- Privacy audit logs
- Quality metric dashboards
- ML model performance benchmarks

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Your Name**
- GitHub: [@yourusername](https://github.com/yourusername)
- LinkedIn: [Your Profile](https://linkedin.com/in/yourprofile)

## 🙏 Acknowledgments

- SDV (Synthetic Data Vault) library
- Google Differential Privacy
- Hugging Face Transformers

---

**⭐ If you find this project useful, please give it a star!**

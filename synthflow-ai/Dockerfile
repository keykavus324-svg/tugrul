# SynthFlow Docker Configuration

FROM python:3.9-slim

WORKDIR /app

# Install system dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# Copy requirements first for better caching
COPY requirements.txt .

# Install Python dependencies
RUN pip install --no-cache-dir -r requirements.txt

# Copy source code
COPY src/ ./src/
COPY workflows/ ./workflows/
COPY tests/ ./tests/

# Create data directories
RUN mkdir -p /app/data /app/output /app/logs

# Set environment variables
ENV PYTHONPATH=/app
ENV PYTHONUNBUFFERED=1

# Default command
CMD ["python", "-c", "from src import SyntheticDataGenerator; print('SynthFlow Ready!')"]

# For running tests
RUN echo '#!/bin/bash\npytest tests/ -v --cov=src' > /app/run_tests.sh
RUN chmod +x /app/run_tests.sh

# For running pipeline
RUN echo '#!/bin/bash\npython -m prefect flow-run' > /app/run_pipeline.sh
RUN chmod +x /app/run_pipeline.sh

EXPOSE 8080

LABEL maintainer="your.email@example.com"
LABEL description="SynthFlow - Synthetic Data Generation Pipeline"
LABEL version="1.0.0"

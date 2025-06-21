FROM arm64v8/debian:bullseye-slim

# Metadata
LABEL maintainer="Sahrul Gunawan <developer.limited.id@gmail.com>"
LABEL version="3.6"
LABEL description="Thunder Linux Power Edition container - aarch64"

# Install dependencies (if needed)
RUN apt-get update && apt-get install -y \
    curl \
    ca-certificates \
    xz-utils \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /opt/thunder

# Download Thunder Linux rootfs
ADD https://github.com/SahrulGunawan-ID/Thunder-Linux-Power-Edition/releases/download/Thunder-Linux/thunder.tar.xz .

# Extract rootfs
RUN tar -xJf thunder.tar.xz && chmod +x ./thunder

# Default command
CMD ["./thunder"]

# Build stage
FROM debian:bookworm-slim AS builder
WORKDIR /app

# Install compiler and MySQL Connector C++
RUN apt-get update && apt-get install -y \
    g++ \
    libmysqlcppconn-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Copy the entire project
COPY . .

# Compile the project 
# Note: On Linux we use -lpthread for cpp-httplib and -lmysqlcppconn for database
RUN g++ main.cpp \
    config/database_connection.cpp \
    services/admin_service.cpp \
    services/student_service.cpp \
    services/faculty_service.cpp \
    -o college_app \
    -lpthread \
    -lmysqlcppconn \
    -std=c++11

# Final stage
FROM debian:bookworm-slim
WORKDIR /app

# Install MySQL Connector runtime libraries
RUN apt-get update && apt-get install -y \
    libmysqlcppconn-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Copy the executable and the public folder from builder
COPY --from=builder /app/college_app .
COPY --from=builder /app/public ./public

# Expose port
EXPOSE 8080

# Run the app
CMD ["./college_app"]

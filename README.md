# Web Server for Inverted Index

This project implements a web server for managing and searching text files using an inverted index. It combines efficient C++ structures for backend processing with a modern FastAPI-based web API. Users can upload text files, search for specific text in those files, and delete files.

---

## Features

- **Text File Upload:** Users can upload text files to the server.
- **Search Functionality:** Efficiently search for terms within uploaded files using an inverted index.
- **File Deletion:** Delete specific files stored on the server.
- **Concurrency Support:** The server is designed to handle multiple requests simultaneously, leveraging multithreading and optimized data structures.

---

## Installation

1. **Clone the Repository:**
    ```
    git clone https://github.com/lilarin/AI-Moderation-Blog-API.git
    ```
2. **Create a Virtual Environment:**
    ```
    python -m venv env
    source env/bin/activate
    ```

3. **Launch Docker Application:**

4. **Run the Docker-Compose in the Project Terminal:**
    ```
    docker-compose up --build
    ```

---

## Core Technologies

### **C++ Backend**
- **Multithreading:**
  - Implements a custom thread pool to handle multiple client requests concurrently.
  - Uses synchronization primitives such as `std::shared_mutex` and `std::condition_variable` to ensure thread-safe operations.
- **Inverted Index Implementation:**
  - Optimized data structure to map terms to the documents containing them.
  - Supports fast addition of documents and term searching.
- **File Storage Management:**
  - Handles saving, reading, and deleting uploaded files.
  - Includes locking mechanisms for concurrent access safety.

### **FastAPI Web API**
- **Endpoints:**
  - `POST /api`: Uploads a file to the server.
  - `GET /api`: Searches for the content in the uploaded files.
  - `DELETE /api`: Deletes a file by name.
- **Swagger UI:** Provides an interactive interface for API testing and documentation.

### **Python Client and Testing**
- **Async Client:**
  - Built using `asyncio` for simulating multiple concurrent users efficiently.
- **Load Testing Scripts:**
  - Simulate concurrent user behavior and measure server performance.

### **C++ Client**
- A lightweight C++ client capable of interfacing with the server for basic operations.

---

## API Endpoints

### **1. Upload File**
- **Method:** `POST`
- **Endpoint:** `/api`
- **Description:** Uploads a text file to the server for indexing.
- **Parameters:**
  - `filename`: The text filename to be uploaded.
  - `content`: The text content to be uploaded.

### **2. Search Files**
- **Method:** `GET`
- **Endpoint:** `/api`
- **Description:** Searches for specific terms in the uploaded files.
- **Parameters:**
  - `query`: The term to search files for.

### **3. Delete File**
- **Method:** `DELETE`
- **Endpoint:** `/api`
- **Description:** Deletes a specified file from the server.
- **Parameters:**
  - `filename`: The name of the file to delete.

---

## Project Structure

### **Backend (C++):**
- `main.cpp`: Entry point of the server application.
- `web_server.cpp/.h`: Manages HTTP request handling and routing.
- `thread_pool.cpp/.h`: Implements a thread pool for concurrent task execution.
- `inverted_index.cpp/.h`: Defines the inverted index structure and its operations.
- `file_storage.cpp/.h`: Handles file system operations (save, read, delete).
- `utils.cpp/.h`: Utility functions shared across modules.

### **Web API (Python):**
- `main.py`: FastAPI application defining endpoints and API logic.

### **Clients:**
- `client.py`: Python client library for interacting with the server.
- `load_test.py`: Python script for concurrent user simulation and load testing.
- `cpp_client.cpp`: Lightweight C++ client for server interaction.

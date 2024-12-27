# Parallel computing coursework – Web Server for Inverted Index

This project implements a web server for searching text files using an inverted index. The server is designed for efficient handling of concurrent requests by leveraging parallel processing and optimized data structures.  It allows users to upload text files, search for specific terms within these files, and delete files.

## Core Technologies

* **C++:** The core server logic, including indexing, searching, and file storage, is implemented in C++ for performance.  Key libraries and features used include:
    * **Standard Template Library (STL):**  `<string>`, `<vector>`, `<set>`, `<mutex>`, `<condition_variable>`, `<thread>`, `<shared_mutex>` are utilized for efficient data manipulation, synchronization, and parallel processing.
    * **Multithreading:** The server employs a custom thread pool (`thread_pool.cpp/.h`) to handle concurrent client requests, maximizing resource utilization and improving response times.
    * **Inverted Index (`inverted_index.cpp/.h`):**  A highly optimized inverted index data structure is used for efficient text searching. This structure maps terms to the documents containing those terms, enabling fast retrieval of search results.
    * **File Storage (`file_storage.cpp/.h`):**  Handles file system operations for storing and retrieving uploaded files. Implements locking mechanisms for safe concurrent access.
    * **Custom HTTP Request Parsing:**  The server directly parses HTTP requests for better control and performance compared to using a full-fledged HTTP library.
* **Python (asyncio):** The client and load testing scripts are written in Python, utilizing the `asyncio` library for asynchronous communication with the server. This allows for simulating multiple concurrent users efficiently.  The `requests` library is used for simpler synchronous testing.

## Prerequisites

* **C++ Compiler:** A modern C++ compiler supporting C++17 or later.
* **Python 3.12:** With the `asyncio`, `requests` and `matplotlib` libraries installed.

## Project Structure
* **main.cpp:** The main entry point of the C++ server application. Initializes the server and starts listening for connections.
* **web_server.cpp/.h:** Handles incoming HTTP requests, dispatches them to the appropriate handlers, and sends responses back to the client.
* **thread_pool.cpp/.h:** Implements the thread pool, managing worker threads and distributing tasks among them.
* **inverted_index.cpp/.h:** Contains the implementation of the inverted index data structure and related operations (adding documents, searching).
* **indexer.cpp/.h:** Responsible for building and updating the inverted index from the uploaded files.
* **file_storage.cpp/.h:** Manages file storage on the server, including saving, reading, and deleting files.
* **utils.cpp/.h:** Utility functions used across the project.
* **main.py:** Example Python client demonstrating basic interactions with the server.
* **client.py:** Python client library providing functions for communicating with the server asynchronously.
* **load_test.py:** Python script for simulating concurrent users and performing load testing.
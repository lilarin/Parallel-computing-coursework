import asyncio
from http import HTTPStatus

from client import Client


async def main():
    client = Client("localhost", 8080)

    # Test files upload
    filename = "test1.txt"
    content = "This is the content of test file 1."
    status, data = await client.upload_file(filename, content)
    print(f"Upload file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")

    filename = "test2.txt"
    content = "This is the content of test file 2."
    status, data = await client.upload_file(filename, content)
    print(f"Upload file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")

    # Test searching
    status, data = await client.search_index("content")
    print(f"Search. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")

    # Test searching non-existent query
    status, data = await client.search_index("12333333qsadadasdad")
    print(
        f"Search with non-existing query. Status: {status} ({HTTPStatus(status).phrase}), data: {data}"
    )

    # Deleting first file
    status, data = await client.delete_file("test1.txt")
    print(f"Delete file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")

    # Test searching after deleting single file
    status, data = await client.search_index("content")
    print(
        f"Search non-existent file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}"
    )

    # Deleting second file
    status, data = await client.delete_file("test2.txt")
    print(f"Delete file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")

    # Test searching after deleting the both files
    status, data = await client.search_index("content")
    print(
        f"Search non-existent file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}"
    )

    # Test deleting a non-existent file
    status, data = await client.delete_file("nonexistent.txt")
    print(
        f"Delete non-existent file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}"
    )


if __name__ == "__main__":
    asyncio.run(main())

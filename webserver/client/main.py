import asyncio
from http import HTTPStatus

from webserver.client.client import client


async def main():
    try:
        # Test files upload
        filename = "test1.txt"
        content = "This is the content of test file 1."
        status, data = await client.upload_file(filename, content)
        print(f"Upload file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")
        await asyncio.sleep(0.5)

        filename = "test2.txt"
        content = "This is the content of test file 2."
        status, data = await client.upload_file(filename, content)
        print(f"Upload file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")
        await asyncio.sleep(0.5)

        # Test searching
        status, data = await client.search_files("content")
        print(f"Search. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")
        await asyncio.sleep(0.5)

        # Test searching non-existent query
        status, data = await client.search_files("12333333qsadadasdad")
        print(
            f"Search with non-existing query. Status: {status} ({HTTPStatus(status).phrase}), data: {data}"
        )
        await asyncio.sleep(0.5)

        # Deleting first file
        status, data = await client.delete_file("test1.txt")
        print(f"Delete file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")
        await asyncio.sleep(0.5)

        # Test searching after deleting single file
        status, data = await client.search_files("content")
        print(
            f"Search non-existent file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}"
        )
        await asyncio.sleep(0.5)

        # Deleting second file
        status, data = await client.delete_file("test2.txt")
        print(f"Delete file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}")
        await asyncio.sleep(0.5)

        # Test searching after deleting the both files
        status, data = await client.search_files("content")
        print(
            f"Search non-existent file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}"
        )
        await asyncio.sleep(0.5)

        # Test deleting a non-existent file
        status, data = await client.delete_file("nonexistent.txt")
        print(
            f"Delete non-existent file. Status: {status} ({HTTPStatus(status).phrase}), data: {data}"
        )
    except (ConnectionRefusedError, ConnectionResetError) as exception:
        print(exception)
    except Exception as exception:
        print(exception)

if __name__ == "__main__":
    asyncio.run(main())

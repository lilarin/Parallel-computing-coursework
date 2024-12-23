import asyncio
import random
import string

from client import Client, RequestType

async def generate_words(length: int = 50):
    return [
        "".join(random.choices(string.ascii_letters + string.digits, k=5))
        for _ in range(length)
    ]

async def choice_words(words: str, amount: int = 5):
    return " ".join(random.sample(words, amount))

async def generate_filename(length: int = 5):
    return "".join(random.choices(string.ascii_letters + string.digits, k=length)) + ".txt"

async def client_task(client, words, num_requests, created_files, request_stats, request_stats_lock, files_lock):
    for i in range(num_requests):
        action = random.choice(
            [RequestType.UPLOAD_FILES, RequestType.SEARCH, RequestType.DELETE_FILE]
        )

        start_time = asyncio.get_running_loop().time()

        if action == RequestType.UPLOAD_FILES:
            filename = await generate_filename()
            content = await choice_words(words)
            status, _ = await client.upload_file(filename, content)
            end_time = asyncio.get_running_loop().time()
            duration = end_time - start_time
            async with request_stats_lock:
                request_stats['upload']['total_time'] += duration
                request_stats['upload']['count'] += 1
            if status == 201:
                async with files_lock:
                    created_files.add(filename)

        elif action == RequestType.SEARCH:
            content = await choice_words(words)
            search_term = random.choice(content.split())
            _, _ = await client.search_index(search_term)
            end_time = asyncio.get_running_loop().time()
            duration = end_time - start_time
            async with request_stats_lock:
                request_stats['search']['total_time'] += duration
                request_stats['search']['count'] += 1

        elif action == RequestType.DELETE_FILE:
            async with files_lock:
                if created_files:
                    file_to_delete = random.choice(list(created_files))
                else:
                    file_to_delete = "non-existing.txt"

            status, _ = await client.delete_file(file_to_delete)
            end_time = asyncio.get_running_loop().time()
            duration = end_time - start_time
            async with request_stats_lock:
                request_stats['delete']['total_time'] += duration
                request_stats['delete']['count'] += 1
            if status == 200 and file_to_delete != "non-existing.txt":
                async with files_lock:
                    created_files.remove(file_to_delete)

async def clean(client, created_files, files_lock):
    async with files_lock:
        clear_tasks = [
            client.delete_file(filename)
            for filename in created_files
        ]
    await asyncio.gather(*clear_tasks)

async def main():
    client = Client("localhost", 8080)
    words = await generate_words()
    num_clients = 1000
    num_requests = 5

    created_files = set()
    request_stats = {
        'upload': {'total_time': 0, 'count': 0},
        'search': {'total_time': 0, 'count': 0},
        'delete': {'total_time': 0, 'count': 0}
    }
    request_stats_lock = asyncio.Lock()
    files_lock = asyncio.Lock()

    client_tasks = [
        client_task(client, words, num_requests, created_files, request_stats, request_stats_lock, files_lock)
        for _ in range(num_clients)
    ]
    await asyncio.gather(*client_tasks)

    total_requests = num_clients * num_requests
    total_time = sum(stats['total_time'] for stats in request_stats.values())
    overall_average_time = total_time / total_requests
    print(f"Загальний середній час запиту (по всіх типах): {overall_average_time:.4f} секунд")

    await clean(client, created_files, files_lock)

if __name__ == "__main__":
    asyncio.run(main())
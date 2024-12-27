import asyncio
import random
import string

import matplotlib.pyplot as plt

from client import Client, RequestType

async def generate_words(length: int = 50):
    return [
        "".join(random.choices(string.ascii_letters + string.digits, k=5))
        for _ in range(length)
    ]

async def choice_words(words: str, amount: int = 5):
    return " ".join(random.sample(words, amount))

async def generate_filename(length: int = 5):
    return (
        "".join(random.choices(string.ascii_letters + string.digits, k=length)) + ".txt"
    )

async def client_task(
    client,
    words,
    num_requests,
    created_files,
    request_stats,
    request_stats_lock,
    files_lock,
):
    async def single_request():
        action = random.choice(
            [RequestType.UPLOAD_FILES, RequestType.SEARCH, RequestType.DELETE_FILE]
        )
        start_time = asyncio.get_running_loop().time()
        status = None  # Initialize status
        response = None # Initialize response

        if action == RequestType.UPLOAD_FILES:
            filename = await generate_filename()
            content = await choice_words(words)
            status, response = await client.upload_file(filename, content)
            end_time = asyncio.get_running_loop().time()
            duration = end_time - start_time
            async with request_stats_lock:
                if status is not None:
                    request_stats["upload"]["total_time"] += duration
                    request_stats["upload"]["count"] += 1
                    if status == 201:
                        async with files_lock:
                            created_files.add(filename)
                else:
                    request_stats["failed"]["count"] += 1

        elif action == RequestType.SEARCH:
            content = await choice_words(words)
            search_term = random.choice(content.split())
            status, response = await client.search_index(search_term)
            end_time = asyncio.get_running_loop().time()
            duration = end_time - start_time
            async with request_stats_lock:
                if status is not None:
                    request_stats["search"]["total_time"] += duration
                    request_stats["search"]["count"] += 1
                else:
                    request_stats["failed"]["count"] += 1

        elif action == RequestType.DELETE_FILE:
            async with files_lock:
                file_to_delete = (
                    random.choice(list(created_files))
                    if created_files
                    else "non-existing.txt"
                )
            status, response = await client.delete_file(file_to_delete)
            end_time = asyncio.get_running_loop().time()
            duration = end_time - start_time
            async with request_stats_lock:
                if status is not None:
                    request_stats["delete"]["total_time"] += duration
                    request_stats["delete"]["count"] += 1
                    if status == 200 and file_to_delete != "non-existing.txt":
                        async with files_lock:
                            created_files.remove(file_to_delete)
                else:
                    request_stats["failed"]["count"] += 1
        await asyncio.sleep(random.uniform(0, 2))

    await asyncio.gather(*[single_request() for _ in range(num_requests)])

async def clean(client, created_files, files_lock):
    async with files_lock:
        clear_tasks = [client.delete_file(filename) for filename in created_files]
    await asyncio.gather(*clear_tasks)

async def run_test(num_clients: int, num_requests: int):
    client = Client("localhost", 8080)
    words = await generate_words()

    created_files = set()
    request_stats = {
        "upload": {"total_time": 0.0, "count": 0},
        "search": {"total_time": 0.0, "count": 0},
        "delete": {"total_time": 0.0, "count": 0},
        "failed": {"count": 0},
    }
    request_stats_lock = asyncio.Lock()
    files_lock = asyncio.Lock()

    client_tasks = [
        client_task(
            client,
            words,
            num_requests,
            created_files,
            request_stats,
            request_stats_lock,
            files_lock,
        )
        for _ in range(num_clients)
    ]
    await asyncio.gather(*client_tasks)

    await clean(client, created_files, files_lock)

    avg_times = {}
    for action, stats in request_stats.items():
        if action != "failed" and stats["count"] > 0:
            avg_times[action] = stats["total_time"] / stats["count"]
        elif action != "failed":
            avg_times[action] = 0.0
    failed_count = request_stats["failed"]["count"]
    return avg_times, failed_count

async def main():
    user_counts = [5, 10, 15, 25, 35]

    requests_per_user = 25
    all_avg_times = {}
    all_failed_counts = {}

    for num_users in user_counts:
        print(f"Running test for {num_users} users...")
        avg_times, failed_count = await run_test(num_users, requests_per_user)
        all_avg_times[num_users] = avg_times
        all_failed_counts[num_users] = failed_count

    # Prepare data for plotting
    users = list(all_avg_times.keys())
    upload_times = [all_avg_times[u].get("upload", 0) for u in users]
    search_times = [all_avg_times[u].get("search", 0) for u in users]
    delete_times = [all_avg_times[u].get("delete", 0) for u in users]
    failed_requests = list(all_failed_counts.values())

    # Create the figure and subplots
    fig, axs = plt.subplots(2, 1, figsize=(10, 10))

    # Plot average request times on the first subplot
    axs[0].plot(users, upload_times, marker="o", label="Upload")
    axs[0].plot(users, search_times, marker="o", label="Search")
    axs[0].plot(users, delete_times, marker="o", label="Delete")
    axs[0].set_xlabel("Number of Users")
    axs[0].set_ylabel("Average Request Time (seconds)")
    axs[0].set_title("Average Request Time vs. Number of Users")
    axs[0].set_xticks(users)
    axs[0].legend()
    axs[0].grid(True)

    # Plot failed requests on the second subplot
    axs[1].plot(users, failed_requests, marker="x", linestyle='--', color='red', label="Failed Requests")
    axs[1].set_xlabel("Number of Users")
    axs[1].set_ylabel("Number of Failed Requests")
    axs[1].set_title("Number of Failed Requests vs. Number of Users")
    axs[1].set_xticks(users)
    axs[1].legend()
    axs[1].grid(True)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    asyncio.run(main())
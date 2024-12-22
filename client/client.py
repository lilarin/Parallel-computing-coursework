import asyncio
import struct
import logging

class RequestType:
    UPLOAD_FILES = 0
    SEARCH = 1
    DELETE_FILE = 2

class Client:
    def __init__(self, host, port):
        self.host = host
        self.port = port

    async def _send_request(self, request_type, data=""):
        try:
            reader, writer = await asyncio.open_connection(self.host, self.port)
            encoded_data = data.encode("utf-8")
            request = struct.pack("B", request_type) + encoded_data
            writer.write(request)
            await writer.drain()

            response_data = await reader.read(1024)
            if not response_data:
                logging.error("No response received from server.")
                writer.close()
                await writer.wait_closed()
                return

            response_parts = response_data.decode(
                "utf-8", errors="ignore"
            ).split('\n', 1)
            if len(response_parts) != 2:
                logging.error(f"Invalid response format: {response_parts}")
                writer.close()
                await writer.wait_closed()
                return

            status_code, response_text = response_parts
            writer.close()
            await writer.wait_closed()
            return int(status_code), response_text

        except ConnectionRefusedError:
            logging.error(f"Connection refused by server at {self.host}:{self.port}")
            return
        except Exception as error:
            logging.error(f"An error occurred during communication: {error}")
            return

    async def upload_file(self, filename, content):
        data = f"{filename}\n{content}"
        return await self._send_request(RequestType.UPLOAD_FILES, data)

    async def search_index(self, query):
        return await self._send_request(RequestType.SEARCH, query)

    async def delete_file(self, filename):
        return await self._send_request(RequestType.DELETE_FILE, filename)

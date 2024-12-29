import asyncio
import struct

class RequestType:
    UPLOAD_FILES = 0
    SEARCH = 1
    DELETE_FILE = 2

class Client:
    def __init__(self, host, port):
        self.host = host
        self.port = port

    async def _send_request(self, request_type, data=""):
        writer = None
        try:
            reader, writer = await asyncio.open_connection(
                self.host, self.port, ssl=False
            )
            encoded_data = data.encode("utf-8")
            request = struct.pack("B", request_type) + encoded_data
            writer.write(request)
            await writer.drain()

            response_data = await reader.read(1024)
            response_parts = response_data.decode("utf-8").split("\n", 1)
            status_code, response_text = response_parts
            return int(status_code), response_text
        except ConnectionRefusedError:
            raise ConnectionRefusedError(f"Connection refused by server at {self.host}:{self.port}")
        except ConnectionResetError:
            raise ConnectionRefusedError(f"Connection reset by server at {self.host}:{self.port}")
        except Exception:
            raise Exception(f"Server is not available")
        finally:
            if writer is not None:
                writer.close()
                await writer.wait_closed()

    async def upload_file(self, filename, content):
        data = f"{filename}\n{content}"
        return await self._send_request(RequestType.UPLOAD_FILES, data)

    async def search_files(self, query):
        return await self._send_request(RequestType.SEARCH, query)

    async def delete_file(self, filename):
        return await self._send_request(RequestType.DELETE_FILE, filename)


client = Client("backend", 8080) # to work properly in docker
# client = Client("0.0.0.0", 8080)

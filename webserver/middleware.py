from functools import wraps

from fastapi.responses import JSONResponse

from webserver.schemas import ServerResponse

def parse_response_middleware(func):
    @wraps(func)
    async def wrapper(*args, **kwargs):
        try:
            result = await func(*args, **kwargs)
            response = ServerResponse(response=result.response)
            return JSONResponse(content=response.model_dump(), status_code=result.status_code)
        except Exception as exception:
            response = ServerResponse(response=str(exception))
            return JSONResponse(content=response.model_dump(), status_code=500)
    return wrapper

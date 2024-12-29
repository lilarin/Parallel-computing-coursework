from typing import Optional

from pydantic import BaseModel


class ServerRawResponse(BaseModel):
    response: str
    status_code: int


class ServerResponse(BaseModel):
    response: Optional[str] = None


class UploadFile(BaseModel):
    filename: str
    content: str

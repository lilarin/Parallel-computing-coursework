from pydantic import BaseModel


class ServerResponse(BaseModel):
    status_code: int
    response: str


class UploadFile(BaseModel):
    filename: str
    content: str

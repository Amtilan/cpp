# Streaming_Kairat - Админская панель CRUD

Полное руководство по созданию и использованию административной панели для управления всеми сущностями платформы Streaming_Kairat.

## 📋 Содержание

- [Обзор архитектуры](#обзор-архитектуры)
- [Структура моделей](#структура-моделей)
- [CRUD операции](#crud-операции)
- [API Endpoints](#api-endpoints)
- [Примеры использования](#примеры-использования)
- [Схемы данных](#схемы-данных)
- [Настройка и запуск](#настройка-и-запуск)

## Обзор архитектуры

Административная панель построена на основе FastAPI с использованием SQLModel для ORM и поддерживает полный CRUD для всех сущностей системы.

### Архитектура системы

```mermaid
graph TB
    A[Admin Panel] --> B[FastAPI Routes]
    B --> C[Service Layer]
    C --> D[Repository Layer]
    D --> E[Database Models]
    
    E --> E1[Story]
    E --> E2[Reels]
    E --> E3[Video]
    E --> E4[Photo]
    E --> E5[Like]
    E --> E6[Comment]
    E --> E7[View]
    
    C --> C1[StoryService]
    C --> C2[ReelsService]
    C --> C3[VideoService]
    C --> C4[PhotoService]
    C --> C5[LikeService]
    C --> C6[CommentService]
    C --> C7[ViewService]
```

### Связи между моделями

```mermaid
erDiagram
    Story ||--o{ Photo : contains
    Story ||--o| Video : "may have"
    Story ||--o{ Like : receives
    Story ||--o{ Comment : receives
    Story ||--o{ View : tracks
    
    Reels ||--|| Video : contains
    Reels ||--o{ Like : receives
    Reels ||--o{ Comment : receives
    Reels ||--o{ View : tracks
    
    Photo }o--|| Story : "belongs to"
    Video }o--o| Story : "belongs to"
    Video }o--o| Reels : "belongs to"
    
    Story {
        int id PK
        string en_title
        string kk_title
        string ru_title
        string en_description
        string kk_description
        string ru_description
        string filename
        string original_path
        string status
        int size
        string media_type
        datetime created_at
        datetime updated_at
    }
    
    Reels {
        int id PK
        string en_title
        string kk_title
        string ru_title
        string en_description
        string kk_description
        string ru_description
        string filename
        string original_path
        string status
        int size
        string media_type
        datetime created_at
        datetime updated_at
    }
    
    Video {
        int id PK
        string en_title
        string kk_title
        string ru_title
        string hls_path
        string thumbnail_path
        float duration
        int story_id FK
        int reels_id FK
        datetime created_at
    }
    
    Photo {
        int id PK
        string en_title
        string kk_title
        string ru_title
        int width
        int height
        int story_id FK
        datetime created_at
    }
    
    Like {
        int id PK
        int user_id
        int story_id FK
        int reels_id FK
        datetime created_at
    }
    
    Comment {
        int id PK
        int user_id
        int story_id FK
        int reels_id FK
        string comment
        datetime created_at
    }
    
    View {
        int id PK
        int user_id
        int story_id FK
        int reels_id FK
        datetime created_at
    }
```

## Структура моделей

### Базовые классы

```python
# BaseModel - содержит id, created_at, updated_at
# MediaMixin - содержит поля для мультиязычного контента
class MediaMixin:
    en_title: str
    kk_title: str  
    ru_title: str
    en_description: str
    kk_description: str
    ru_description: str
    filename: str
    original_path: str
    status: MediaStatus
    size: int
    media_type: MediaType
    error_message: Optional[str]
```

### Основные модели

1. **Story** - Истории (коллекции фото с опциональным видео)
2. **Reels** - Короткие видеоролики  
3. **Video** - Видеофайлы
4. **Photo** - Фотографии
5. **Like** - Лайки пользователей
6. **Comment** - Комментарии пользователей
7. **View** - Просмотры контента

## CRUD операции

### Stories CRUD

```python
# web/api/api_v1/endpoints/admin/stories.py

from fastapi import APIRouter, Depends, HTTPException, UploadFile, File, Form
from typing import List, Optional
from web.apps.stories.service import StoryService
from web.api.deps import get_story_service

router = APIRouter(prefix="/admin/stories", tags=["admin-stories"])

@router.post("/")
async def create_story(
    files: List[UploadFile] = File(...),
    en_title: str = Form(...),
    kk_title: str = Form(...),
    ru_title: str = Form(...),
    en_description: str = Form(...),
    kk_description: str = Form(...),
    ru_description: str = Form(...),
    story_service: StoryService = Depends(get_story_service)
):
    """Создание новой истории с файлами"""
    return await story_service.create_story(
        files=files,
        media_type=MediaType.STORIES,
        en_title=en_title,
        kk_title=kk_title,
        ru_title=ru_title,
        en_description=en_description,
        kk_description=kk_description,
        ru_description=ru_description
    )

@router.get("/")
async def list_stories(
    limit: int = 20,
    offset: int = 0,
    story_service: StoryService = Depends(get_story_service)
):
    """Получение списка всех историй"""
    return await story_service.get_stories()

@router.get("/{story_id}")
async def get_story(
    story_id: int,
    story_service: StoryService = Depends(get_story_service)
):
    """Получение истории по ID"""
    return await story_service.get_story(story_id)

@router.put("/{story_id}")
async def update_story(
    story_id: int,
    update_data: StoryUpdateSchema,
    story_service: StoryService = Depends(get_story_service)
):
    """Обновление метаданных истории"""
    return await story_service.update_story_metadata(story_id, update_data)

@router.delete("/{story_id}")
async def delete_story(
    story_id: int,
    story_service: StoryService = Depends(get_story_service)
):
    """Удаление истории"""
    success = await story_service.remove_story(story_id)
    if not success:
        raise HTTPException(status_code=404, detail="Story not found")
    return {"message": "Story deleted successfully"}
```

### Reels CRUD

```python
# web/api/api_v1/endpoints/admin/reels.py

@router.post("/")
async def create_reel(
    file: UploadFile = File(...),
    en_title: str = Form(...),
    kk_title: str = Form(...),
    ru_title: str = Form(...),
    en_description: str = Form(...),
    kk_description: str = Form(...),
    ru_description: str = Form(...),
    reels_service: ReelsService = Depends(get_reels_service)
):
    """Создание нового рилса"""
    return await reels_service.create_reel(
        file=file,
        media_type=MediaType.REELS,
        en_title=en_title,
        kk_title=kk_title,
        ru_title=ru_title,
        en_description=en_description,
        kk_description=kk_description,
        ru_description=ru_description
    )

@router.get("/")
async def list_reels(
    params: ReelListQueryParams = Depends(),
    reels_service: ReelsService = Depends(get_reels_service)
):
    """Получение списка всех рилсов"""
    return await reels_service.get_reels(params)

@router.get("/{reels_id}")
async def get_reel(
    reels_id: int,
    reels_service: ReelsService = Depends(get_reels_service)
):
    """Получение рилса по ID"""
    return await reels_service.get_reel(reels_id)

@router.put("/{reels_id}")
async def update_reel(
    reels_id: int,
    update_data: ReelUpdateSchema,
    reels_service: ReelsService = Depends(get_reels_service)
):
    """Обновление метаданных рилса"""
    return await reels_service.update_reel_metadata(reels_id, update_data)

@router.delete("/{reels_id}")
async def delete_reel(
    reels_id: int,
    reels_service: ReelsService = Depends(get_reels_service)
):
    """Удаление рилса"""
    success = await reels_service.remove_reel(reels_id)
    if not success:
        raise HTTPException(status_code=404, detail="Reel not found")
    return {"message": "Reel deleted successfully"}
```

## API Endpoints

### Полная структура endpoints

```mermaid
graph TD
    A[Admin API] --> B[Stories]
    A --> C[Reels]
    A --> D[Videos]
    A --> E[Photos]
    A --> F[Likes]
    A --> G[Comments]
    A --> H[Views]
    
    B --> B1["POST /admin/stories/"]
    B --> B2["GET /admin/stories/"]
    B --> B3["GET /admin/stories/{id}"]
    B --> B4["PUT /admin/stories/{id}"]
    B --> B5["DELETE /admin/stories/{id}"]
    
    C --> C1["POST /admin/reels/"]
    C --> C2["GET /admin/reels/"]
    C --> C3["GET /admin/reels/{id}"]
    C --> C4["PUT /admin/reels/{id}"]
    C --> C5["DELETE /admin/reels/{id}"]
    
    D --> D1["POST /admin/videos/"]
    D --> D2["GET /admin/videos/"]
    D --> D3["GET /admin/videos/{id}"]
    D --> D4["PUT /admin/videos/{id}/status"]
    D --> D5["DELETE /admin/videos/{id}"]
    
    E --> E1["POST /admin/photos/"]
    E --> E2["GET /admin/photos/"]
    E --> E3["GET /admin/photos/{id}"]
    E --> E4["PUT /admin/photos/{id}/status"]
    E --> E5["DELETE /admin/photos/{id}"]
```

### Таблица всех endpoints

| Сущность | Метод | Endpoint | Описание |
|----------|-------|----------|----------|
| **Stories** | POST | `/admin/stories/` | Создание истории |
| | GET | `/admin/stories/` | Список историй |
| | GET | `/admin/stories/{id}` | Получение истории |
| | PUT | `/admin/stories/{id}` | Обновление истории |
| | DELETE | `/admin/stories/{id}` | Удаление истории |
| **Reels** | POST | `/admin/reels/` | Создание рилса |
| | GET | `/admin/reels/` | Список рилсов |
| | GET | `/admin/reels/{id}` | Получение рилса |
| | PUT | `/admin/reels/{id}` | Обновление рилса |
| | DELETE | `/admin/reels/{id}` | Удаление рилса |
| **Videos** | POST | `/admin/videos/` | Создание видео |
| | GET | `/admin/videos/` | Список видео |
| | GET | `/admin/videos/{id}` | Получение видео |
| | PUT | `/admin/videos/{id}/status` | Обновление статуса |
| | DELETE | `/admin/videos/{id}` | Удаление видео |
| **Photos** | POST | `/admin/photos/` | Создание фото |
| | GET | `/admin/photos/` | Список фото |
| | GET | `/admin/photos/{id}` | Получение фото |
| | PUT | `/admin/photos/{id}/status` | Обновление статуса |
| | DELETE | `/admin/photos/{id}` | Удаление фото |
| **Likes** | POST | `/admin/likes/` | Создание лайка |
| | GET | `/admin/likes/` | Список лайков |
| | GET | `/admin/likes/{id}` | Получение лайка |
| | DELETE | `/admin/likes/{id}` | Удаление лайка |
| **Comments** | POST | `/admin/comments/` | Создание комментария |
| | GET | `/admin/comments/` | Список комментариев |
| | GET | `/admin/comments/{id}` | Получение комментария |
| | DELETE | `/admin/comments/{id}` | Удаление комментария |
| **Views** | POST | `/admin/views/` | Создание просмотра |
| | GET | `/admin/views/` | Список просмотров |
| | GET | `/admin/views/{id}` | Получение просмотра |
| | DELETE | `/admin/views/{id}` | Удаление просмотра |

## Схемы данных

### Схемы запросов и ответов

```python
# web/api/api_v1/endpoints/admin/schemas.py

from pydantic import BaseModel
from typing import Optional, List
from datetime import datetime
from web.apps.models import MediaStatus, MediaType

# Story Schemas
class StoryResponse(BaseModel):
    id: int
    en_title: str
    kk_title: str
    ru_title: str
    en_description: str
    kk_description: str
    ru_description: str
    filename: str
    original_path: str
    status: MediaStatus
    size: int
    media_type: MediaType
    error_message: Optional[str]
    preview_thumbnail_path: Optional[str]
    created_at: datetime
    updated_at: datetime
    photos: List["PhotoResponse"] = []
    video: Optional["VideoResponse"] = None
    likes_count: int
    comments_count: int
    views_count: int

class StoryUpdateSchema(BaseModel):
    en_title: Optional[str] = None
    kk_title: Optional[str] = None
    ru_title: Optional[str] = None
    en_description: Optional[str] = None
    kk_description: Optional[str] = None
    ru_description: Optional[str] = None
    preview_thumbnail_path: Optional[str] = None

# Reel Schemas
class ReelResponse(BaseModel):
    id: int
    en_title: str
    kk_title: str
    ru_title: str
    en_description: str
    kk_description: str
    ru_description: str
    filename: str
    original_path: str
    status: MediaStatus
    size: int
    media_type: MediaType
    error_message: Optional[str]
    preview_thumbnail_path: Optional[str]
    created_at: datetime
    updated_at: datetime
    video: Optional["VideoResponse"] = None
    likes_count: int
    comments_count: int
    views_count: int

class ReelUpdateSchema(BaseModel):
    en_title: Optional[str] = None
    kk_title: Optional[str] = None
    ru_title: Optional[str] = None
    en_description: Optional[str] = None
    kk_description: Optional[str] = None
    ru_description: Optional[str] = None
    preview_thumbnail_path: Optional[str] = None

# Video Schemas
class VideoResponse(BaseModel):
    id: int
    en_title: str
    kk_title: str
    ru_title: str
    en_description: str
    kk_description: str
    ru_description: str
    filename: str
    original_path: str
    status: MediaStatus
    size: int
    media_type: MediaType
    error_message: Optional[str]
    hls_path: Optional[str]
    thumbnail_path: Optional[str]
    duration: Optional[float]
    story_id: Optional[int]
    reels_id: Optional[int]
    created_at: datetime
    updated_at: datetime

# Photo Schemas
class PhotoResponse(BaseModel):
    id: int
    en_title: str
    kk_title: str
    ru_title: str
    en_description: str
    kk_description: str
    ru_description: str
    filename: str
    original_path: str
    status: MediaStatus
    size: int
    media_type: MediaType
    error_message: Optional[str]
    compressed_path: Optional[str]
    thumbnail_path: Optional[str]
    width: int
    height: int
    story_id: int
    created_at: datetime
    updated_at: datetime

# Like Schemas
class LikeResponse(BaseModel):
    id: int
    user_id: int
    story_id: Optional[int]
    reels_id: Optional[int]
    created_at: datetime
    updated_at: datetime

# Comment Schemas
class CommentResponse(BaseModel):
    id: int
    user_id: int
    story_id: Optional[int]
    reels_id: Optional[int]
    comment: str
    created_at: datetime
    updated_at: datetime

# View Schemas
class ViewResponse(BaseModel):
    id: int
    user_id: int
    story_id: Optional[int]
    reels_id: Optional[int]
    created_at: datetime
    updated_at: datetime
```

## Примеры использования

### Создание истории

```bash
curl -X POST "http://localhost:8000/admin/stories/" \
  -H "Content-Type: multipart/form-data" \
  -F "files=@photo1.jpg" \
  -F "files=@photo2.jpg" \
  -F "files=@video.mp4" \
  -F "en_title=My Story" \
  -F "kk_title=Менің ауызайым" \
  -F "ru_title=Моя история" \
  -F "en_description=Amazing story about my day" \
  -F "kk_description=Менің күнім туралы керемет тарих" \
  -F "ru_description=Удивительная история о моем дне"
```

### Получение списка историй

```bash
curl -X GET "http://localhost:8000/admin/stories/?limit=10&offset=0"
```

### Создание рилса

```bash
curl -X POST "http://localhost:8000/admin/reels/" \
  -H "Content-Type: multipart/form-data" \
  -F "file=@reel_video.mp4" \
  -F "en_title=My Reel" \
  -F "kk_title=Менің рилсым" \
  -F "ru_title=Мой рилс" \
  -F "en_description=Cool video reel" \
  -F "kk_description=Тамаша видео рилс" \
  -F "ru_description=Крутой видео рилс"
```

### Получение аналитики

```bash
# Получение лайков для истории
curl -X GET "http://localhost:8000/admin/likes/?story_id=1"

# Получение комментариев для рилса
curl -X GET "http://localhost:8000/admin/comments/?reels_id=1"

# Получение просмотров пользователя
curl -X GET "http://localhost:8000/admin/views/?user_id=1"
```

## Поток данных в админке

```mermaid
sequenceDiagram
    participant Admin as Admin User
    participant API as FastAPI Router
    participant Service as Service Layer
    participant DB as Database
    participant Cache as Redis Cache
    participant S3 as S3 Storage
    
    Admin->>API: POST /admin/stories/ (with files)
    API->>Service: create_story()
    Service->>DB: Create Story record
    Service->>S3: Upload media files
    Service->>DB: Create Photo/Video records
    Service->>Cache: Cache story data
    Service->>API: Return created story
    API->>Admin: Story response
    
    Admin->>API: GET /admin/stories/
    API->>Service: get_stories()
    Service->>Cache: Check cache
    alt Cache hit
        Cache->>Service: Return cached data
    else Cache miss
        Service->>DB: Query stories
        DB->>Service: Return stories
        Service->>Cache: Update cache
    end
    Service->>API: Return stories list
    API->>Admin: Stories list response
    
    Admin->>API: DELETE /admin/stories/{id}
    API->>Service: remove_story()
    Service->>DB: Delete records
    Service->>S3: Delete files
    Service->>Cache: Clear cache
    Service->>API: Return success
    API->>Admin: Deletion confirmation
```

## Настройка и запуск

### 1. Создание файлов

```bash
# Создание директорий для админки
mkdir -p web/api/api_v1/endpoints/admin

# Создание файлов endpoints
touch web/api/api_v1/endpoints/admin/__init__.py
touch web/api/api_v1/endpoints/admin/stories.py
touch web/api/api_v1/endpoints/admin/reels.py
touch web/api/api_v1/endpoints/admin/videos.py
touch web/api/api_v1/endpoints/admin/photos.py
touch web/api/api_v1/endpoints/admin/likes.py
touch web/api/api_v1/endpoints/admin/comments.py
touch web/api/api_v1/endpoints/admin/views.py
touch web/api/api_v1/endpoints/admin/schemas.py
```

### 2. Обновление главного роутера

```python
# web/api/api_v1/api.py

from fastapi import APIRouter
from web.api.api_v1.endpoints.admin import (
    stories as admin_stories,
    reels as admin_reels,
    videos as admin_videos,
    photos as admin_photos,
    likes as admin_likes,
    comments as admin_comments,
    views as admin_views,
)

api_router = APIRouter()

# Существующие роуты...

# Админские роуты
api_router.include_router(admin_stories.router, prefix="/admin", tags=["admin"])
api_router.include_router(admin_reels.router, prefix="/admin", tags=["admin"])
api_router.include_router(admin_videos.router, prefix="/admin", tags=["admin"])
api_router.include_router(admin_photos.router, prefix="/admin", tags=["admin"])
api_router.include_router(admin_likes.router, prefix="/admin", tags=["admin"])
api_router.include_router(admin_comments.router, prefix="/admin", tags=["admin"])
api_router.include_router(admin_views.router, prefix="/admin", tags=["admin"])
```

### 3. Запуск приложения

```bash
# Запуск сервера разработки
uvicorn web.main:app --reload --host 0.0.0.0 --port 8000
```

### 4. Документация API

После запуска приложения, документация будет доступна по адресу:
- Swagger UI: `http://localhost:8000/docs`
- ReDoc: `http://localhost:8000/redoc`

## Основные особенности

### 🏗️ **Архитектура**
- Слоевая архитектура с разделением ответственности
- Сервисы для бизнес-логики
- Репозитории для работы с данными

### 📚 **Полный CRUD**
- Создание с загрузкой файлов
- Чтение с фильтрацией и пагинацией
- Обновление метаданных
- Удаление с очисткой ресурсов

### 🌐 **Мультиязычность**
- Поддержка 3 языков: английский, казахский, русский
- Отдельные поля для каждого языка

### 🚀 **Интеграции**
- S3 для хранения файлов
- Redis для кэширования
- PostgreSQL для данных
- FFMPEG для обработки видео

Эта админская панель предоставляет полный CRUD функционал для всех сущностей системы с поддержкой мультиязычности, файлового хранилища S3 и кэширования Redis.

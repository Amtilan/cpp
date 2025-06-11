# API Documentation - Streaming Kairat Platform

Полная документация API для платформы Streaming Kairat с примерами запросов и тел ответов.

## Структура API

API построен на базе FastAPI и поддерживает следующие основные модули:
- **Stories** - короткие истории с фото/видео
- **Reels** - короткие видео ролики  
- **Videos** - работа с видео контентом
- **Photos** - работа с изображениями
- **Likes** - лайки к контенту
- **Comments** - комментарии к контенту
- **Views** - просмотры контента
- **HLS** - стриминг видео

## Authentication

В данный момент API не требует аутентификации, но использует `user_id` в качестве идентификатора пользователя.

---

## 1. Stories API

### 1.1 Создание Stories

**POST** `/api/v1/stories/upload`

Загружает файлы для создания Stories с мультиязычной поддержкой.

**Параметры:**
- `files` (file[]) - массив файлов для загрузки
- `en_title` (string, max 100) - заголовок на английском
- `kk_title` (string, max 100) - заголовок на казахском  
- `ru_title` (string, max 100) - заголовок на русском
- `en_description` (string, max 1000) - описание на английском
- `kk_description` (string, max 1000) - описание на казахском
- `ru_description` (string, max 1000) - описание на русском

**Пример запроса:**
```bash
curl -X POST "http://localhost:8000/api/v1/stories/upload" \
  -H "Content-Type: multipart/form-data" \
  -F "files=@story1.mp4" \
  -F "files=@story2.jpg" \
  -F "en_title=My Story" \
  -F "kk_title=Менің Тарихым" \
  -F "ru_title=Моя История" \
  -F "en_description=This is my story description" \
  -F "kk_description=Бұл менің тарих сипаттамасы" \
  -F "ru_description=Это описание моей истории"
```

**Ответ:**
```json
{
  "id": 1,
  "photo_ids": [1, 2],
  "video_id": 1,
  "status": "processing",
  "message": "Сторис успешно создан",
  "en_title": "My Story",
  "kk_title": "Менің Тарихым", 
  "ru_title": "Моя История",
  "en_description": "This is my story description",
  "kk_description": "Бұл менің тарих сипаттамасы",
  "ru_description": "Это описание моей истории",
  "preview_thumbnail_path": "/path/to/thumbnail.jpg"
}
```

### 1.2 Получение списка Stories

**GET** `/api/v1/stories/list?user_id={user_id}`

**Параметры запроса:**
- `user_id` (int, optional) - ID пользователя для определения лайков

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/stories/list?user_id=123"
```

**Ответ:**
```json
[
  {
    "id": 1,
    "photos": [
      {
        "id": 1,
        "filename": "photo1.jpg"
      }
    ],
    "video": {
      "id": 1,
      "filename": "video1.mp4",
      "status": "ready",
      "error_message": null,
      "en_title": "My Story",
      "kk_title": "Менің Тарихым",
      "ru_title": "Моя История"
    },
    "status": "ready",
    "created_at": "2024-01-15T10:30:00Z",
    "updated_at": "2024-01-15T10:35:00Z",
    "error_message": null,
    "likes_count": 15,
    "preview_thumbnail_path": "/path/to/thumbnail.jpg",
    "is_liked": true
  }
]
```

### 1.3 Получение детальной информации Stories

**GET** `/api/v1/stories/{story_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/stories/1"
```

**Ответ:** (аналогичен элементу из списка Stories)

### 1.4 Статус обработки видео

**GET** `/api/v1/stories/status/{video_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/stories/status/1"
```

**Ответ:**
```json
{
  "id": 1,
  "filename": "video1.mp4",
  "status": "ready",
  "error_message": null,
  "en_title": "My Story",
  "kk_title": "Менің Тарихым",
  "ru_title": "Моя История",
  "en_description": "Description...",
  "kk_description": "Сипаттама...",
  "ru_description": "Описание..."
}
```

### 1.5 Загрузка превью Stories

**POST** `/api/v1/stories/preview/{story_id}`

**Параметры:**
- `file` (file) - файл превью

**Пример запроса:**
```bash
curl -X POST "http://localhost:8000/api/v1/stories/preview/1" \
  -H "Content-Type: multipart/form-data" \
  -F "file=@preview.jpg"
```

**Ответ:**
```json
{
  "id": 1,
  "preview_thumbnail_path": "/path/to/new_preview.jpg",
  "message": "Превью успешно загружено"
}
```

### 1.6 Получение превью Stories

**GET** `/api/v1/stories/preview/{story_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/stories/preview/1"
```

### 1.7 Обновление метаданных Stories

**PATCH** `/api/v1/stories/stories/{story_id}/metadata`

**Тело запроса:**
```json
{
  "en_title": "Updated Title",
  "kk_title": "Жаңартылған Тақырып",
  "ru_title": "Обновленный Заголовок",
  "en_description": "Updated description",
  "kk_description": "Жаңартылған сипаттама", 
  "ru_description": "Обновленное описание"
}
```

**Пример запроса:**
```bash
curl -X PATCH "http://localhost:8000/api/v1/stories/stories/1/metadata" \
  -H "Content-Type: application/json" \
  -d '{
    "en_title": "Updated Title",
    "ru_title": "Обновленный Заголовок"
  }'
```

### 1.8 Удаление Stories

**DELETE** `/api/v1/stories/remove/{story_id}/`

**Пример запроса:**
```bash
curl -X DELETE "http://localhost:8000/api/v1/stories/remove/1/"
```

**Ответ:**
```json
{
  "message": "Сторис успешно удален"
}
```

---

## 2. Reels API

### 2.1 Создание Reels

**POST** `/api/v1/reels/reels/upload`

Загружает видео для создания Reels с мультиязычной поддержкой.

**Параметры:**
- `file` (file) - видео файл
- `en_title` (string, max 100) - заголовок на английском
- `kk_title` (string, max 100) - заголовок на казахском
- `ru_title` (string, max 100) - заголовок на русском
- `en_description` (string, max 1000) - описание на английском
- `kk_description` (string, max 1000) - описание на казахском
- `ru_description` (string, max 1000) - описание на русском

**Пример запроса:**
```bash
curl -X POST "http://localhost:8000/api/v1/reels/reels/upload" \
  -H "Content-Type: multipart/form-data" \
  -F "file=@reel_video.mp4" \
  -F "en_title=My Reel" \
  -F "kk_title=Менің Reel-ым" \
  -F "ru_title=Мой Reel" \
  -F "en_description=Amazing reel content" \
  -F "kk_description=Керемет reel мазмұны" \
  -F "ru_description=Потрясающий контент reel"
```

**Ответ:**
```json
{
  "id": 1,
  "video_id": 1,
  "status": "processing",
  "message": "Reels успешно создан",
  "en_title": "My Reel",
  "kk_title": "Менің Reel-ым",
  "ru_title": "Мой Reel",
  "en_description": "Amazing reel content",
  "kk_description": "Керемет reel мазмұны",
  "ru_description": "Потрясающий контент reel",
  "preview_thumbnail_path": "/path/to/thumbnail.jpg"
}
```

### 2.2 Получение списка Reels

**GET** `/api/v1/reels/reels/list`

**Параметры запроса:**
- `user_id` (int, optional) - ID пользователя для определения лайков
- `limit` (int, default=20, max=100) - количество записей
- `offset` (int, default=0) - смещение для пагинации
- `sort_by` (string) - сортировка: "created_at", "likes", "views"
- `order` (string) - порядок: "asc", "desc"

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/reels/reels/list?user_id=123&limit=10&sort_by=likes&order=desc"
```

**Ответ:**
```json
{
  "items": [
    {
      "id": 1,
      "video_id": 1,
      "status": "ready",
      "created_at": "2024-01-15T10:30:00Z",
      "updated_at": "2024-01-15T10:35:00Z",
      "error_message": null,
      "en_title": "My Reel",
      "kk_title": "Менің Reel-ым",
      "ru_title": "Мой Reel",
      "likes_count": 25,
      "comment_count": 8,
      "preview_thumbnail_path": "/path/to/thumbnail.jpg",
      "comments": [
        {
          "user_id": 456,
          "text": "Great reel!",
          "created_at": "2024-01-15T11:00:00Z"
        }
      ],
      "is_liked": true
    }
  ]
}
```

### 2.3 Получение детальной информации Reels

**GET** `/api/v1/reels/reels/{reels_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/reels/reels/1"
```

**Ответ:**
```json
{
  "id": 1,
  "video": {
    "id": 1,
    "filename": "reel_video.mp4",
    "status": "ready",
    "error_message": null,
    "en_title": "My Reel",
    "kk_title": "Менің Reel-ым",
    "ru_title": "Мой Reel"
  },
  "status": "ready",
  "created_at": "2024-01-15T10:30:00Z",
  "updated_at": "2024-01-15T10:35:00Z",
  "error_message": null,
  "likes_count": 25,
  "comment_count": 8,
  "preview_thumbnail_path": "/path/to/thumbnail.jpg",
  "comments": [
    {
      "user_id": 456,
      "text": "Great reel!",
      "created_at": "2024-01-15T11:00:00Z"
    }
  ]
}
```

### 2.4 Загрузка превью Reels

**POST** `/api/v1/reels/reels/preview/{reels_id}`

**Параметры:**
- `file` (file) - файл превью

**Пример запроса:**
```bash
curl -X POST "http://localhost:8000/api/v1/reels/reels/preview/1" \
  -H "Content-Type: multipart/form-data" \
  -F "file=@reel_preview.jpg"
```

### 2.5 Получение превью Reels

**GET** `/api/v1/reels/reels/preview/{reels_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/reels/reels/preview/1"
```

### 2.6 Обновление метаданных Reels

**PATCH** `/api/v1/reels/reels/{reels_id}/metadata`

**Тело запроса:**
```json
{
  "en_title": "Updated Reel Title",
  "kk_title": "Жаңартылған Reel Тақырыбы",
  "ru_title": "Обновленный заголовок Reel",
  "en_description": "Updated reel description",
  "kk_description": "Жаңартылған reel сипаттамасы",
  "ru_description": "Обновленное описание reel"
}
```

### 2.7 Удаление Reels

**DELETE** `/api/v1/reels/reels/remove/{reels_id}/`

**Пример запроса:**
```bash
curl -X DELETE "http://localhost:8000/api/v1/reels/reels/remove/1/"
```

**Ответ:**
```json
{
  "message": "Reels успешно удален"
}
```

---

## 3. Likes API

### 3.1 Добавление лайка

**POST** `/api/v1/likes/add`

**Параметры:**
- `user_id` (int) - ID пользователя
- `story_id` (int, optional) - ID Stories (либо story_id, либо reels_id)
- `reels_id` (int, optional) - ID Reels

**Пример запроса:**
```bash
curl -X POST "http://localhost:8000/api/v1/likes/add" \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "user_id=123&story_id=1"
```

**Ответ:**
```json
{
  "id": 1,
  "user_id": 123,
  "story_id": 1,
  "reels_id": null,
  "message": "Лайк успешно добавлен"
}
```

### 3.2 Удаление лайка

**DELETE** `/api/v1/likes/remove`

**Параметры:**
- `user_id` (int) - ID пользователя  
- `story_id` (int, optional) - ID Stories
- `reels_id` (int, optional) - ID Reels

**Пример запроса:**
```bash
curl -X DELETE "http://localhost:8000/api/v1/likes/remove" \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "user_id=123&story_id=1"
```

**Ответ:**
```json
{
  "message": "Лайк успешно удалён"
}
```

---

## 4. Comments API

### 4.1 Добавление комментария

**POST** `/api/v1/comments/add`

**Параметры:**
- `user_id` (int) - ID пользователя
- `story_id` (int, optional) - ID Stories
- `reels_id` (int, optional) - ID Reels  
- `comment` (string) - текст комментария

**Пример запроса:**
```bash
curl -X POST "http://localhost:8000/api/v1/comments/add" \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "user_id=123&story_id=1&comment=Отличная история!"
```

**Ответ:**
```json
{
  "id": 1,
  "user_id": 123,
  "story_id": 1,
  "reels_id": null,
  "comment": "Отличная история!",
  "message": "Комментарий успешно добавлен"
}
```

### 4.2 Получение списка комментариев

**GET** `/api/v1/comments/list`

**Параметры запроса:**
- `story_id` (int, optional) - ID Stories
- `reels_id` (int, optional) - ID Reels
- `limit` (int, default=20, max=100) - количество комментариев
- `offset` (int, default=10) - смещение для пагинации

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/comments/list?story_id=1&limit=5&offset=0"
```

**Ответ:**
```json
{
  "count": 3,
  "comments": [
    {
      "id": 1,
      "user_id": 123,
      "story_id": 1,
      "reels_id": null,
      "comment": "Отличная история!"
    },
    {
      "id": 2,
      "user_id": 456,
      "story_id": 1,
      "reels_id": null,
      "comment": "Очень понравилось!"
    }
  ]
}
```

### 4.3 Удаление комментария

**DELETE** `/api/v1/comments/remove/{comment_id}`

**Пример запроса:**
```bash
curl -X DELETE "http://localhost:8000/api/v1/comments/remove/1"
```

**Ответ:**
```json
{
  "message": "Комментарий успешно удалён"
}
```

---

## 5. Views API

### 5.1 Добавление просмотра

**POST** `/api/v1/views/add`

**Параметры:**
- `user_id` (int) - ID пользователя
- `story_id` (int, optional) - ID Stories
- `reels_id` (int, optional) - ID Reels

**Пример запроса:**
```bash
curl -X POST "http://localhost:8000/api/v1/views/add" \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "user_id=123&story_id=1"
```

**Ответ:**
```json
{
  "id": 1,
  "user_id": 123,
  "story_id": 1,
  "reels_id": null,
  "message": "Просмотр успешно добавлен"
}
```

### 5.2 Получение списка просмотров

**GET** `/api/v1/views/list`

**Параметры запроса:**
- `story_id` (int, optional) - ID Stories
- `reels_id` (int, optional) - ID Reels
- `limit` (int, default=20, max=100) - максимум записей
- `offset` (int, default=0) - смещение для пагинации

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/views/list?story_id=1&limit=10"
```

**Ответ:**
```json
{
  "count": 5,
  "views": [
    {
      "id": 1,
      "user_id": 123,
      "story_id": 1,
      "reels_id": null
    },
    {
      "id": 2,
      "user_id": 456,
      "story_id": 1,
      "reels_id": null
    }
  ]
}
```

### 5.3 Получение просмотров пользователя

**GET** `/api/v1/views/user/{user_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/views/user/123"
```

**Ответ:**
```json
{
  "count": 10,
  "views": [
    {
      "id": 1,
      "user_id": 123,
      "story_id": 1,
      "reels_id": null
    }
  ]
}
```

### 5.4 Получение просмотров Stories

**GET** `/api/v1/views/story/{story_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/views/story/1"
```

### 5.5 Получение просмотров Reels

**GET** `/api/v1/views/reels/{reels_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/views/reels/1"
```

### 5.6 Удаление просмотра

**DELETE** `/api/v1/views/remove/{view_id}`

**Пример запроса:**
```bash
curl -X DELETE "http://localhost:8000/api/v1/views/remove/1"
```

**Ответ:**
```json
{
  "message": "Просмотр успешно удалён"
}
```

---

## 6. Videos API

### 6.1 Загрузка видео

**POST** `/api/v1/video/upload`

**Параметры:**
- `file` (file) - видео файл
- `title` (string) - заголовок видео
- `description` (string) - описание видео

**Пример запроса:**
```bash
curl -X POST "http://localhost:8000/api/v1/video/upload" \
  -H "Content-Type: multipart/form-data" \
  -F "file=@video.mp4" \
  -F "title=My Video" \
  -F "description=Video description"
```

**Ответ:**
```json
{
  "id": 1,
  "filename": "video.mp4",
  "status": "processing",
  "message": "Видео успешно загружено и отправлено на обработку",
  "title": "My Video",
  "description": "Video description"
}
```

### 6.2 Получение статуса видео

**GET** `/api/v1/video/status/{video_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/video/status/1"
```

**Ответ:**
```json
{
  "id": 1,
  "filename": "video.mp4",
  "status": "ready",
  "error_message": null,
  "title": "My Video",
  "description": "Video description"
}
```

### 6.3 Получение списка видео

**GET** `/api/v1/video/list`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/video/list"
```

**Ответ:**
```json
[
  {
    "id": 1,
    "filename": "video.mp4",
    "status": "ready",
    "created_at": "2024-01-15T10:30:00Z",
    "updated_at": "2024-01-15T10:35:00Z",
    "error_message": "",
    "title": "My Video",
    "description": "Video description"
  }
]
```

### 6.4 Удаление видео

**DELETE** `/api/v1/video/remove/{video_id}/`

**Пример запроса:**
```bash
curl -X DELETE "http://localhost:8000/api/v1/video/remove/1/"
```

**Ответ:**
```json
{
  "message": "Видео успешно удалено"
}
```

---

## 7. Photos API

### 7.1 Загрузка фото

**POST** `/api/v1/photo/upload`

**Параметры:**
- `file` (file) - файл изображения
- `title` (string) - заголовок фото
- `description` (string) - описание фото

**Пример запроса:**
```bash
curl -X POST "http://localhost:8000/api/v1/photo/upload" \
  -H "Content-Type: multipart/form-data" \
  -F "file=@photo.jpg" \
  -F "title=My Photo" \
  -F "description=Photo description"
```

**Ответ:**
```json
{
  "id": 1,
  "filename": "photo.jpg",
  "status": "ready",
  "message": "Фото успешно загружено"
}
```

### 7.2 Получение статуса фото

**GET** `/api/v1/photo/status/{photo_id}`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/photo/status/1"
```

**Ответ:**
```json
{
  "id": 1,
  "filename": "photo.jpg",
  "status": "ready",
  "error_message": null
}
```

### 7.3 Получение списка фото

**GET** `/api/v1/photo/list`

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/photo/list"
```

**Ответ:**
```json
[
  {
    "id": 1,
    "filename": "photo.jpg",
    "status": "ready",
    "title": "My Photo",
    "description": "Photo description",
    "created_at": "2024-01-15T10:30:00Z",
    "updated_at": "2024-01-15T10:30:00Z",
    "error_message": null
  }
]
```

### 7.4 Удаление фото

**DELETE** `/api/v1/photo/remove/{photo_id}/`

**Пример запроса:**
```bash
curl -X DELETE "http://localhost:8000/api/v1/photo/remove/1/"
```

**Ответ:**
```json
{
  "message": "Фото успешно удалено"
}
```

---

## 8. HLS Streaming API

### 8.1 Получение плейлиста HLS

**GET** `/api/v1/hls/playlist/{video_id}`

Возвращает манифест файл для HLS стриминга.

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/hls/playlist/1"
```

### 8.2 Получение сегмента видео

**GET** `/api/v1/hls/video/{video_id}/{segment}`

Возвращает отдельный сегмент видео для HLS воспроизведения.

**Пример запроса:**
```bash
curl -X GET "http://localhost:8000/api/v1/hls/video/1/segment001.ts"
```

---

## Коды статусов

### Статусы обработки медиа:
- `pending` - ожидает обработки
- `processing` - обрабатывается
- `ready` - готово к использованию
- `error` - ошибка при обработке
- `not_found` - файл не найден

### HTTP коды ответов:
- `200` - успешный запрос
- `201` - ресурс создан
- `400` - некорректный запрос  
- `404` - ресурс не найден
- `422` - ошибка валидации данных
- `500` - внутренняя ошибка сервера

---

## Примеры использования

### Создание полноценного Stories с видео и фото:

```bash
# 1. Загружаем Stories
curl -X POST "http://localhost:8000/api/v1/stories/upload" \
  -F "files=@video.mp4" \
  -F "files=@photo1.jpg" \
  -F "files=@photo2.jpg" \
  -F "en_title=My Adventure" \
  -F "kk_title=Менің Шытырман Оқиғам" \
  -F "ru_title=Мое Приключение" \
  -F "en_description=Amazing adventure story" \
  -F "kk_description=Керемет шытырман оқиға тарихы" \
  -F "ru_description=Удивительная история приключений" 

# 2. Проверяем статус обработки
curl -X GET "http://localhost:8000/api/v1/stories/status/1"

# 3. Загружаем превью
curl -X POST "http://localhost:8000/api/v1/stories/preview/1" \
  -F "file=@preview.jpg"
```

### Работа с лайками и комментариями:

```bash
# 1. Ставим лайк
curl -X POST "http://localhost:8000/api/v1/likes/add" \
  -d "user_id=123&story_id=1"

# 2. Добавляем комментарий
curl -X POST "http://localhost:8000/api/v1/comments/add" \
  -d "user_id=123&story_id=1&comment=Отличная история!"

# 3. Отмечаем просмотр
curl -X POST "http://localhost:8000/api/v1/views/add" \
  -d "user_id=123&story_id=1"

# 4. Получаем обновленную информацию
curl -X GET "http://localhost:8000/api/v1/stories/list?user_id=123"
```

### Создание и управление Reels:

```bash
# 1. Создаем Reels
curl -X POST "http://localhost:8000/api/v1/reels/reels/upload" \
  -F "file=@reel.mp4" \
  -F "en_title=Cool Reel" \
  -F "kk_title=Тамаша Reel" \
  -F "ru_title=Крутой Reel" \
  -F "en_description=My cool reel video" \
  -F "kk_description=Менің тамаша reel видеом" \
  -F "ru_description=Мое крутое reel видео"

# 2. Получаем список с сортировкой по лайкам
curl -X GET "http://localhost:8000/api/v1/reels/reels/list?sort_by=likes&order=desc&limit=10"

# 3. Обновляем метаданные
curl -X PATCH "http://localhost:8000/api/v1/reels/reels/1/metadata" \
  -H "Content-Type: application/json" \
  -d '{"en_title": "Updated Cool Reel"}'
```

---

## Многоязычность

Платформа поддерживает три языка:
- **en** - English (английский)
- **kk** - Қазақша (казахский) 
- **ru** - Русский (русский)

Для каждого контента (Stories, Reels) необходимо указывать заголовок и описание на всех трех языках.

---

## Техническая информация

- **Framework**: FastAPI
- **Database**: PostgreSQL  
- **Cache**: Redis
- **Storage**: AWS S3
- **Video Processing**: FFmpeg + Celery
- **Streaming**: HLS (HTTP Live Streaming)

Для получения OpenAPI документации:
- Swagger UI: `http://localhost:8000/docs`
- ReDoc: `http://localhost:8000/redoc`
- OpenAPI JSON: `http://localhost:8000/openapi.json`

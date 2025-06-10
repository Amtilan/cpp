# FC Kairat Backend API

<p align="center">
  <img src="https://nestjs.com/img/logo-small.svg" width="120" alt="Nest Logo" />
</p>

## 📋 Описание

**FC Kairat Backend API** - это комплексный бэкенд-сервис для мобильного приложения футбольного клуба "Кайрат". Сервис предоставляет API для управления пользователями, системой лояльности, квизами, уведомлениями и стриками (сериями достижений).

### 🏗️ Архитектура системы

```mermaid
graph TB
    subgraph "Client Layer"
        A[Mobile App]
        B[Web Admin]
    end
    
    subgraph "Load Balancer"
        C[Nginx/Load Balancer]
    end
    
    subgraph "Application Layer"
        D[NestJS API Server]
        E[Fastify HTTP Server]
    end
    
    subgraph "Background Jobs"
        F[Bull Queues]
        G[Redis]
    end
    
    subgraph "Database Layer"
        H[PostgreSQL]
        I[Prisma ORM]
    end
    
    subgraph "External Services"
        J[Firebase Admin]
        K[Push Notifications]
    end
    
    subgraph "Monitoring"
        L[OpenTelemetry]
        M[Winston Logger]
        N[Loki]
        O[Prometheus]
    end
    
    A --> C
    B --> C
    C --> D
    D --> E
    D --> F
    F --> G
    D --> I
    I --> H
    D --> J
    J --> K
    D --> L
    L --> M
    M --> N
    L --> O
```

### 🚀 Основные возможности

- **👤 Управление пользователями**: Регистрация, аутентификация, профили
- **🎯 Система лояльности**: Начисление/списание баллов, история транзакций
- **📝 Квизы**: Создание и прохождение викторин
- **🔥 Стрики**: Отслеживание серий достижений пользователей
- **📱 Push-уведомления**: Персональные и массовые уведомления через Firebase
- **🌐 Мультиязычность**: Поддержка русского и казахского языков
- **📊 Мониторинг**: Комплексная система логирования и метрик

## 🛠️ Технологический стек

| Категория | Технологии |
|-----------|------------|
| **Framework** | NestJS, Fastify |
| **Language** | TypeScript |
| **Database** | PostgreSQL, Prisma ORM |
| **Cache & Queues** | Redis, Bull/BullMQ |
| **Authentication** | JWT, Passport |
| **Push Notifications** | Firebase Admin SDK |
| **Monitoring** | OpenTelemetry, Winston, Loki, Prometheus |
| **Testing** | Jest, Artillery (stress testing) |
| **DevOps** | Docker, Docker Compose |

## 📡 API Endpoints

### Основные модули

```mermaid
graph LR
    subgraph "API Modules"
        A[Users Module]
        B[Loyalty Module]
        C[Quiz Module]
        D[Notifications Module]
        E[Streaks Module]
        F[Health Module]
    end
    
    subgraph "Core Services"
        G[Auth Service]
        H[Database Service]
        I[Cache Service]
        J[Queue Service]
    end
    
    A --> G
    B --> G
    C --> G
    D --> G
    E --> G
    
    A --> H
    B --> H
    C --> H
    D --> H
    E --> H
    
    A --> I
    B --> I
    C --> I
    
    D --> J
```

### 🔗 Список эндпоинтов

| Модуль | Endpoint | Метод | Описание |
|--------|----------|--------|----------|
| **Health** | `/health` | GET | Проверка состояния сервиса |
| **Users** | `/api/v1/user/info` | GET | Информация о пользователе |
| **Users** | `/api/v1/user/language` | GET/POST | Язык пользователя |
| **Loyalty** | `/api/v1/loyalty/status` | GET | Статус программы лояльности |
| **Loyalty** | `/api/v1/loyalty/add-points` | POST | Начисление баллов |
| **Loyalty** | `/api/v1/loyalty/deduct-points` | POST | Списание баллов |
| **Loyalty** | `/api/v1/loyalty/transactions` | GET | История транзакций |
| **Quizzes** | `/api/v1/quizzes` | GET | Список квизов |
| **Streaks** | `/api/v1/streaks` | GET | Информация о стриках |
| **Notifications** | `/api/v1/notifications/send` | POST | Отправка уведомления |
| **Notifications** | `/api/v1/notifications/send-to-all` | POST | Массовая рассылка |

## 🏁 Быстрый старт

### Предварительные требования

- Node.js 18+
- PostgreSQL 14+
- Redis 6+
- Docker & Docker Compose (опционально)

### 📦 Установка

1. **Клонирование репозитория**
```bash
git clone <repository-url>
cd fc-kairat
```

2. **Установка зависимостей**
```bash
npm install
```

3. **Настройка окружения**
```bash
cp .env.example .env
# Настройте переменные окружения
```

4. **Настройка базы данных**
```bash
# Генерация клиента Prisma
npx prisma generate

# Выполнение миграций
npx prisma migrate deploy
```

5. **Запуск сервиса**
```bash
# Режим разработки
npm run start:dev

# Продакшн режим
npm run start:prod
```

### 🐳 Запуск с Docker

```bash
# Сборка и запуск
docker-compose up -d

# Проверка логов
docker-compose logs -f
```

## ⚙️ Конфигурация

### Переменные окружения

```env
# Database
DATABASE_URL="postgresql://user:password@localhost:5432/kairat_db"

# Redis
REDIS_HOST=localhost
REDIS_PORT=6379

# Firebase
FIREBASE_PROJECT_ID=kairat-app
FIREBASE_CLIENT_EMAIL=firebase-adminsdk-xxx@kairat-app.iam.gserviceaccount.com
FIREBASE_PRIVATE_KEY="-----BEGIN PRIVATE KEY-----\n...\n-----END PRIVATE KEY-----\n"

# JWT
JWT_SECRET=your-secret-key
JWT_EXPIRES_IN=7d

# App
PORT=3000
NODE_ENV=production
```

## 📊 Мониторинг и логирование

### Архитектура мониторинга

```mermaid
graph TD
    subgraph "Application"
        A[NestJS App]
        B[Winston Logger]
    end
    
    subgraph "Telemetry"
        C[OpenTelemetry SDK]
        D[Traces]
        E[Metrics]
        F[Logs]
    end
    
    subgraph "Export"
        G[Prometheus Exporter]
        H[OTLP HTTP Exporter]
        I[Loki Transport]
    end
    
    subgraph "Storage"
        J[Prometheus]
        K[Loki]
        L[Jaeger/Tempo]
    end
    
    A --> B
    A --> C
    C --> D
    C --> E
    B --> F
    
    D --> H
    E --> G
    F --> I
    
    H --> L
    G --> J
    I --> K
```

### Метрики и алерты

- **Производительность**: Время ответа, RPS, использование ресурсов
- **Бизнес-метрики**: Количество активных пользователей, транзакций лояльности
- **Ошибки**: HTTP errors, database errors, external service failures
- **Инфраструктура**: CPU, память, дисковое пространство

## 🧪 Тестирование

### Типы тестов

```mermaid
graph LR
    subgraph "Testing Strategy"
        A[Unit Tests]
        B[Integration Tests]
        C[E2E Tests]
        D[Load Tests]
        E[Stress Tests]
    end
    
    subgraph "Tools"
        F[Jest]
        G[Supertest]
        H[Artillery]
    end
    
    A --> F
    B --> F
    C --> G
    D --> H
    E --> H
```

### Команды тестирования

```bash
# Юнит тесты
npm run test

# E2E тесты
npm run test:e2e

# Покрытие кода
npm run test:cov

# Стресс-тесты
npm run test:stress
```

### Сценарии нагрузочного тестирования

- **Warm-up**: 50 RPS в течение 30 секунд
- **Ramp-up**: 100 RPS в течение 60 секунд  
- **Sustained**: 200 RPS в течение 120 секунд
- **Peak**: 300 RPS в течение 60 секунд

## 🔒 Безопасность

### Меры безопасности

- **Authentication**: JWT токены с истечением срока
- **Authorization**: Role-based access control
- **Rate Limiting**: Throttling для предотвращения abuse
- **Helmet**: Безопасность HTTP заголовков
- **CORS**: Контроль доступа между доменами
- **Input Validation**: Валидация всех входящих данных

## 🚀 Развертывание

### Стратегия развертывания

```mermaid
graph TD
    subgraph "Development"
        A[Local Development]
        B[Feature Branch]
        C[Pull Request]
    end
    
    subgraph "Testing"
        D[Staging Environment]
        E[Integration Tests]
        F[Performance Tests]
    end
    
    subgraph "Production"
        G[Blue-Green Deployment]
        H[Health Checks]
        I[Rollback Strategy]
    end
    
    A --> B
    B --> C
    C --> D
    D --> E
    E --> F
    F --> G
    G --> H
    H --> I
```

### Checklist развертывания

- [ ] Миграции базы данных выполнены
- [ ] Переменные окружения настроены
- [ ] Health checks проходят успешно
- [ ] Мониторинг настроен
- [ ] Backup стратегия активна
- [ ] SSL сертификаты актуальны

## 📈 Производительность

### Рекомендуемые характеристики

| Компонент | Минимум | Рекомендуется |
|-----------|---------|---------------|
| **CPU** | 2 cores | 4+ cores |
| **RAM** | 4GB | 8GB+ |
| **Storage** | 50GB SSD | 100GB+ SSD |
| **Network** | 100Mbps | 1Gbps+ |

### Оптимизации

- **Кэширование**: Redis для часто используемых данных
- **Connection Pooling**: Оптимизация подключений к БД
- **Compression**: Gzip сжатие ответов
- **CDN**: Для статических ресурсов

## 🤝 Вклад в развитие

### Workflow

1. Fork репозитория
2. Создайте feature branch
3. Напишите тесты
4. Реализуйте функционал
5. Создайте Pull Request

### Стандарты кода

- **ESLint**: Статический анализ кода
- **Prettier**: Форматирование кода
- **Conventional Commits**: Стандарт коммитов
- **TypeScript**: Строгая типизация

## 📞 Поддержка

- **Документация**: Подробные комментарии в коде
- **Мониторинг**: Real-time метрики и алерты
- **Логирование**: Структурированные логи с контекстом

## 📄 Лицензия

Этот проект использует лицензию UNLICENSED. Все права защищены.

---

<p align="center">
  Сделано с ❤️ для FC Kairat
</p>

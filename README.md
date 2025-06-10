# FC Kairat Backend API

<p align="center">
  <img src="https://nestjs.com/img/logo-small.svg" width="120" alt="Nest Logo" />
</p>

<p align="center">
  <strong>Комплексный бэкенд-сервис для мобильного приложения ФК "Кайрат"</strong>
</p>

## 📋 Описание проекта

**FC Kairat Backend API** - это высокопроизводительный бэкенд-сервис, построенный на NestJS с использованием Fastify, предназначенный для обслуживания мобильного приложения футбольного клуба "Кайрат". Система включает в себя программу лояльности, систему квизов, предикторы матчей, достижения и комплексную систему уведомлений.

### 🏗️ Архитектура системы

```mermaid
graph TB
    subgraph "Client Applications"
        A[Mobile App iOS/Android]
        B[Web Admin Panel]
        C[External Services]
    end
    
    subgraph "API Gateway Layer"
        D[Fastify HTTP Server]
        E[CORS & Security]
        F[Rate Limiting]
        G[Authentication]
    end
    
    subgraph "Business Logic Layer"
        H1[Users Module]
        H2[Loyalty Module]
        H3[Quiz Module]
        H4[Notifications Module]
        H5[Predictor Module]
        H6[Achievements Module]
        H7[Streaks Module]
        H8[Statistics Module]
    end
    
    subgraph "Data Processing Layer"
        I[Bull Queues]
        J[Event Emitter]
        K[Scheduler]
    end
    
    subgraph "Data Persistence Layer"
        L[PostgreSQL Database]
        M[Prisma ORM]
        N[Redis Cache]
        O[Redis Sessions]
    end
    
    subgraph "External Integrations"
        P[Firebase Admin]
        Q[Push Notifications]
    end
    
    subgraph "Observability Stack"
        R[OpenTelemetry]
        S[Winston Logger]
        T[Prometheus Metrics]
        U[Grafana Loki]
    end
    
    A --> D
    B --> D
    C --> D
    D --> E
    E --> F
    F --> G
    G --> H1
    G --> H2
    G --> H3
    G --> H4
    G --> H5
    G --> H6
    G --> H7
    G --> H8
    
    H1 --> I
    H2 --> I
    H3 --> I
    H4 --> I
    
    I --> J
    J --> K
    
    H1 --> M
    H2 --> M
    H3 --> M
    H4 --> M
    H5 --> M
    H6 --> M
    H7 --> M
    H8 --> M
    
    M --> L
    H1 --> N
    H2 --> N
    H3 --> O
    
    H4 --> P
    P --> Q
    
    D --> R
    R --> S
    R --> T
    S --> U
```

## 🗃️ Модель данных

### Основные сущности системы

```mermaid
erDiagram
    User {
        int id PK
        string username UK
        string email UK
        string bearerToken UK
        enum role
        enum status
        int currentPoints
        int currentLevel
        int currentStreak
        int maxStreak
        string preferredLanguage
        datetime createdAt
        datetime updatedAt
    }
    
    Profile {
        int id PK
        int userId FK
        string name
        string lastName
        string phone
        string address
        string location
        int gender
        datetime birthday
        string avatar
    }
    
    LoyaltyTransaction {
        int id PK
        int userId FK
        enum type
        int points
        string reason
        datetime createdAt
    }
    
    Achievement {
        int id PK
        string img_url
        int pointsReward
        int tier
    }
    
    Quiz {
        int id PK
        boolean isActive
        datetime createdAt
        datetime updatedAt
    }
    
    Question {
        int id PK
        int quizId FK
        enum type
        int order
    }
    
    AnswerOption {
        int id PK
        int questionId FK
        boolean isCorrect
    }
    
    CompletedQuiz {
        int id PK
        string token FK
        int quizId FK
        int score
        int totalQuestions
        datetime completedAt
    }
    
    Notification {
        int id PK
        int userId FK
        string title
        string message
        json data
        boolean seen
        datetime createdAt
    }
    
    PredictorAnswer {
        int id PK
        int userId FK
        string slug
        string score
        string firstScorer
        int goalMinute
        boolean getPoints
    }
    
    Ticket {
        int id PK
        int userId FK
        bigint uid
        string location
        string event
        int row
        int seat
        float totalPrice
        string status
    }
    
    User ||--o| Profile : has
    User ||--o{ LoyaltyTransaction : makes
    User ||--o{ Achievement : earns
    User ||--o{ CompletedQuiz : completes
    User ||--o{ Notification : receives
    User ||--o{ PredictorAnswer : predicts
    User ||--o{ Ticket : owns
    Quiz ||--o{ Question : contains
    Question ||--o{ AnswerOption : has
```

## 🚀 Модули и CRUD операции

### 1. 👤 Users Module

**Назначение**: Управление пользователями, профилями и аутентификацией

#### CRUD операции:

| Операция | HTTP | Endpoint | Описание | Тело запроса | Ответ |
|----------|------|----------|----------|-------------|-------|
| **CREATE** | POST | `/api/v1/auth/register` | Регистрация пользователя | `{username, email, password, profile}` | `{user, token}` |
| **READ** | GET | `/api/v1/user/info` | Получение информации о пользователе | - | `{id, username, email, profile, stats}` |
| **READ** | GET | `/api/v1/users` | Список пользователей (admin) | `?page=1&limit=10&search=` | `{users[], total, page}` |
| **UPDATE** | PUT | `/api/v1/user/profile` | Обновление профиля | `{name, lastName, phone, location, gender, birthday}` | `{profile}` |
| **UPDATE** | PATCH | `/api/v1/user/language` | Изменение языка | `{language: "ru"/"kk"}` | `{preferredLanguage}` |
| **UPDATE** | PATCH | `/api/v1/user/avatar` | Загрузка аватара | `FormData: avatar` | `{avatarUrl}` |
| **DELETE** | DELETE | `/api/v1/user/account` | Удаление аккаунта | - | `{success: true}` |

#### Связанные операции:
```typescript
// Получение статистики пользователя
GET /api/v1/user/stats
Response: {
  currentPoints: number,
  currentLevel: number,
  currentStreak: number,
  maxStreak: number,
  totalQuizzes: number,
  totalAchievements: number,
  leaderboardRank: number
}

// Получение рейтинга пользователей
GET /api/v1/users/leaderboard
Query: ?page=1&limit=10
Response: {
  users: [{
    id: number,
    username: string,
    currentPoints: number,
    currentLevel: number,
    rank: number
  }],
  total: number
}
```

### 2. 🎯 Loyalty Module  

**Назначение**: Система лояльности с баллами, уровнями и транзакциями

#### CRUD операции:

| Операция | HTTP | Endpoint | Описание | Тело запроса | Ответ |
|----------|------|----------|----------|-------------|-------|
| **CREATE** | POST | `/api/v1/loyalty/add-points` | Начисление баллов | `{userId, points, reason, language}` | `{transaction, newBalance}` |
| **CREATE** | POST | `/api/v1/loyalty/deduct-points` | Списание баллов | `{userId, points, reason, language}` | `{transaction, newBalance}` |
| **READ** | GET | `/api/v1/loyalty/status` | Статус программы лояльности | - | `{currentPoints, currentLevel, nextLevel, progress}` |
| **READ** | GET | `/api/v1/loyalty/transactions` | История транзакций | `?page=1&limit=20&type=ADD/DEDUCT` | `{transactions[], total, summary}` |
| **READ** | GET | `/api/v1/loyalty/levels` | Информация об уровнях | - | `{levels[], userLevel}` |

#### Бизнес-логика:
```typescript
// Система уровней
interface LoyaltyLevel {
  level: number;
  title: string;
  description: string;
  requiredPoints: number;
  rewards: {
    achievementId?: number;
    bonusPoints?: number;
    specialAccess?: string[];
  };
}

// Типы транзакций
enum LoyaltyTransactionType {
  ADD = "ADD",      // Начисление баллов
  DEDUCT = "DEDUCT" // Списание баллов
}

// Источники начисления баллов
const POINT_SOURCES = {
  QUIZ_COMPLETION: 10,
  CORRECT_PREDICTION: 15,
  DAILY_LOGIN: 5,
  STREAK_BONUS: 20,
  ACHIEVEMENT_UNLOCK: 25,
  SPECIAL_EVENT: 50
};
```

### 3. 📝 Quiz Module

**Назначение**: Система викторин с вопросами, ответами и результатами

#### CRUD операции:

| Операция | HTTP | Endpoint | Описание | Тело запроса | Ответ |
|----------|------|----------|----------|-------------|-------|
| **CREATE** | POST | `/api/v1/quizzes` | Создание квиза (admin) | `{title, description, questions[], isActive}` | `{quiz}` |
| **READ** | GET | `/api/v1/quizzes` | Список квизов | `?filter=all/completed/uncompleted&lang=ru/kk` | `{quizzes[], stats}` |
| **READ** | GET | `/api/v1/quizzes/:id` | Детали квиза | - | `{quiz, questions[], userProgress}` |
| **UPDATE** | PUT | `/api/v1/quizzes/:id` | Обновление квиза (admin) | `{title, description, questions[], isActive}` | `{quiz}` |
| **DELETE** | DELETE | `/api/v1/quizzes/:id` | Удаление квиза (admin) | - | `{success: true}` |

#### Операции прохождения:
```typescript
// Начало прохождения квиза
POST /api/v1/quizzes/:id/start
Response: {
  session: string,
  quiz: {
    id: number,
    title: string,
    questions: [{
      id: number,
      text: string,
      type: "SINGLE_CHOICE" | "MULTIPLE_CHOICE",
      options: [{
        id: number,
        text: string
      }]
    }]
  }
}

// Отправка ответов
POST /api/v1/quizzes/:id/submit
Body: {
  session: string,
  answers: [{
    questionId: number,
    selectedOptions: number[]
  }]
}
Response: {
  score: number,
  totalQuestions: number,
  correctAnswers: number,
  pointsEarned: number,
  results: [{
    questionId: number,
    isCorrect: boolean,
    correctAnswers: number[]
  }]
}

// История прохождений
GET /api/v1/quizzes/history
Query: ?page=1&limit=10
Response: {
  completions: [{
    id: number,
    quiz: {title: string},
    score: number,
    totalQuestions: number,
    completedAt: string,
    pointsEarned: number
  }],
  stats: {
    totalCompleted: number,
    averageScore: number,
    bestScore: number
  }
}
```

### 4. 📱 Notifications Module

**Назначение**: Персональные и массовые push-уведомления через Firebase

#### CRUD операции:

| Операция | HTTP | Endpoint | Описание | Тело запроса | Ответ |
|----------|------|----------|----------|-------------|-------|
| **CREATE** | POST | `/api/v1/notifications/send` | Отправка уведомления | `{userId, title, message, data?}` | `{notification, sent: boolean}` |
| **CREATE** | POST | `/api/v1/notifications/send-to-all` | Массовая рассылка | `{title, message, data?, filter?}` | `{totalSent: number, failed: number}` |
| **CREATE** | POST | `/api/v1/notifications/tokens` | Регистрация токена | `{token, platform}` | `{success: true}` |
| **READ** | GET | `/api/v1/notifications` | Список уведомлений | `?page=1&limit=20&seen=true/false` | `{notifications[], unreadCount}` |
| **UPDATE** | PATCH | `/api/v1/notifications/:id/read` | Пометить как прочитанное | - | `{success: true}` |
| **UPDATE** | PATCH | `/api/v1/notifications/read-all` | Пометить все как прочитанные | - | `{updated: number}` |
| **DELETE** | DELETE | `/api/v1/notifications/:id` | Удаление уведомления | - | `{success: true}` |

#### Firebase Integration:
```typescript
// Типы уведомлений
interface NotificationPayload {
  title: string;
  body: string;
  data?: {
    type: 'quiz' | 'loyalty' | 'achievement' | 'predictor' | 'general';
    targetId?: string;
    action?: string;
    deepLink?: string;
  };
}

// Фильтры для массовой рассылки
interface BroadcastFilter {
  minLevel?: number;
  maxLevel?: number;
  language?: 'ru' | 'kk';
  hasAchievement?: number;
  lastActiveAfter?: Date;
  roles?: ('USER' | 'ADMIN')[];
}

// Статистика отправки
GET /api/v1/notifications/stats
Response: {
  totalSent: number,
  successRate: number,
  byType: {
    quiz: number,
    loyalty: number,
    achievement: number,
    general: number
  },
  byPeriod: {
    today: number,
    thisWeek: number,
    thisMonth: number
  }
}
```

### 5. 🎲 Predictor Module

**Назначение**: Система предикторов для матчей с подсчетом очков

#### CRUD операции:

| Операция | HTTP | Endpoint | Описание | Тело запроса | Ответ |
|----------|------|----------|----------|-------------|-------|
| **CREATE** | POST | `/api/v1/predictor/answer` | Отправка прогноза | `{slug, score, firstScorer, goalMinute, totalShots, shotsOnTarget}` | `{prediction, deadline}` |
| **READ** | GET | `/api/v1/predictor/matches` | Доступные матчи | `?status=upcoming/completed` | `{matches[], userPredictions}` |
| **READ** | GET | `/api/v1/predictor/results/:slug` | Результаты матча | - | `{match, correctAnswers, userAnswers, points}` |
| **READ** | GET | `/api/v1/predictor/leaderboard` | Рейтинг предикторов | `?period=week/month/season` | `{users[], userRank}` |
| **UPDATE** | PUT | `/api/v1/predictor/answer/:slug` | Изменение прогноза | `{score, firstScorer, goalMinute, totalShots, shotsOnTarget}` | `{prediction}` |

#### Система подсчета очков:
```typescript
// Правила начисления очков
const PREDICTOR_SCORING = {
  EXACT_SCORE: 25,      // Точный счет
  CORRECT_WINNER: 15,   // Правильный победитель
  CORRECT_DRAW: 10,     // Правильная ничья
  FIRST_SCORER: 20,     // Первый забивший
  GOAL_MINUTE: 15,      // Минута гола (±5 мин)
  SHOTS_RANGE: 10,      // Удары (±2)
  SHOTS_ON_TARGET: 10   // Удары в створ (±1)
};

// Получение статистики предиктора
GET /api/v1/predictor/user-stats
Response: {
  totalPredictions: number,
  correctPredictions: number,
  accuracy: number,
  totalPoints: number,
  rank: number,
  bestStreak: number,
  currentStreak: number,
  byCompetition: {
    [competition: string]: {
      predictions: number,
      correct: number,
      points: number
    }
  }
}
```

### 6. 🏆 Achievements Module

**Назначение**: Система достижений с многоязычной поддержкой

#### CRUD операции:

| Операция | HTTP | Endpoint | Описание | Тело запроса | Ответ |
|----------|------|----------|----------|-------------|-------|
| **CREATE** | POST | `/api/v1/achievements` | Создание достижения (admin) | `{img_url, pointsReward, tier, translations}` | `{achievement}` |
| **READ** | GET | `/api/v1/achievements` | Список достижений | `?tier=1/2/3&unlocked=true/false&lang=ru/kk` | `{achievements[], userProgress}` |
| **READ** | GET | `/api/v1/achievements/:id` | Детали достижения | - | `{achievement, userStatus, requirements}` |
| **UPDATE** | PUT | `/api/v1/achievements/:id` | Обновление достижения (admin) | `{img_url, pointsReward, tier, translations}` | `{achievement}` |
| **DELETE** | DELETE | `/api/v1/achievements/:id` | Удаление достижения (admin) | - | `{success: true}` |

#### Система разблокировки:
```typescript
// Типы достижений
const ACHIEVEMENT_TYPES = {
  QUIZ_MASTER: 'Завершение квизов',
  PREDICTOR_GURU: 'Точные прогнозы',
  LOYALTY_CHAMPION: 'Накопление баллов',
  STREAK_KEEPER: 'Серии активности',
  SOCIAL_BUTTERFLY: 'Социальная активность',
  NEWCOMER: 'Достижения для новичков',
  VETERAN: 'Долгосрочные цели'
};

// Автоматическая разблокировка
POST /api/v1/achievements/check-unlock
Body: {
  userId: number,
  triggerType: 'quiz_completed' | 'prediction_correct' | 'points_earned' | 'streak_updated',
  context: any
}
Response: {
  unlockedAchievements: [{
    id: number,
    title: string,
    description: string,
    pointsReward: number,
    tier: number
  }]
}

// Прогресс по достижениям
GET /api/v1/achievements/progress
Response: {
  categories: [{
    name: string,
    achievements: [{
      id: number,
      title: string,
      description: string,
      progress: number,
      maxProgress: number,
      isUnlocked: boolean,
      unlockedAt?: string
    }]
  }],
  totalUnlocked: number,
  totalAvailable: number
}
```

### 7. 🔥 Streaks Module

**Назначение**: Отслеживание серий активности пользователей

#### CRUD операции:

| Операция | HTTP | Endpoint | Описание | Тело запроса | Ответ |
|----------|------|----------|----------|-------------|-------|
| **READ** | GET | `/api/v1/streaks` | Информация о стриках | - | `{current, max, lastActivity, bonuses}` |
| **READ** | GET | `/api/v1/streaks/leaderboard` | Рейтинг стриков | `?period=current/all-time` | `{users[], userRank}` |
| **UPDATE** | POST | `/api/v1/streaks/update` | Обновление активности | `{activityType}` | `{newStreak, bonusEarned}` |

#### Система стриков:
```typescript
// Типы активности для стриков
const STREAK_ACTIVITIES = {
  DAILY_LOGIN: 'Ежедневный вход',
  QUIZ_COMPLETION: 'Прохождение квиза',
  PREDICTION_MADE: 'Создание прогноза',
  ACHIEVEMENT_UNLOCK: 'Разблокировка достижения'
};

// Бонусы за стрики
const STREAK_BONUSES = {
  7: { points: 50, achievement: 'WEEK_WARRIOR' },
  14: { points: 100, achievement: 'TWO_WEEK_CHAMPION' },
  30: { points: 250, achievement: 'MONTHLY_MASTER' },
  100: { points: 500, achievement: 'CENTURION' },
  365: { points: 1000, achievement: 'YEAR_LEGEND' }
};

// Календарь активности
GET /api/v1/streaks/calendar
Query: ?year=2024&month=1
Response: {
  days: [{
    date: string,
    hasActivity: boolean,
    activityTypes: string[],
    streakDay: number
  }],
  monthStats: {
    activeDays: number,
    longestStreak: number,
    currentMonthStreak: number
  }
}
```

### 8. 📊 Statistics Module

**Назначение**: Аналитика и статистика системы

#### Операции чтения:

```typescript
// Общая статистика системы
GET /api/v1/statistics/overview
Response: {
  users: {
    total: number,
    active: number,
    newThisMonth: number
  },
  engagement: {
    averageSessionTime: number,
    dailyActiveUsers: number,
    retentionRate: number
  },
  content: {
    totalQuizzes: number,
    completedQuizzes: number,
    totalPredictions: number,
    accuratePredictions: number
  },
  loyalty: {
    totalPointsIssued: number,
    averageUserPoints: number,
    topLevelUsers: number
  }
}

// Аналитика пользователей
GET /api/v1/statistics/users
Query: ?period=day/week/month&segment=new/active/retained
Response: {
  timeSeries: [{
    date: string,
    count: number,
    growth: number
  }],
  demographics: {
    byLanguage: { ru: number, kk: number },
    byLevel: { [level: string]: number },
    byLocation: { [location: string]: number }
  }
}

// Статистика контента
GET /api/v1/statistics/content
Response: {
  quizzes: {
    totalCompleted: number,
    averageScore: number,
    popularQuizzes: [{
      id: number,
      title: string,
      completions: number,
      averageScore: number
    }]
  },
  predictions: {
    totalMade: number,
    accuracy: number,
    topPredictors: [{
      userId: number,
      username: string,
      accuracy: number,
      totalPoints: number
    }]
  }
}
```

## 🛠️ Технологический стек

| Категория | Технологии | Версия | Назначение |
|-----------|------------|---------|------------|
| **Framework** | NestJS | ^11.0.1 | Основной фреймворк |
| **HTTP Server** | Fastify | ^5.3.3 | Высокопроизводительный HTTP сервер |
| **Language** | TypeScript | ^5.7.3 | Строгая типизация |
| **Database** | PostgreSQL | 14+ | Основная база данных |
| **ORM** | Prisma | ^6.4.1 | Object-Relational Mapping |
| **Cache** | Redis | 6+ | Кэширование и сессии |
| **Queue** | BullMQ | ^11.0.2 | Фоновые задачи |
| **Auth** | JWT + Passport | ^11.0.0 | Аутентификация |
| **Push Notifications** | Firebase Admin | ^13.2.0 | Мобильные уведомления |
| **Validation** | Class Validator | ^0.14.1 | Валидация данных |
| **Documentation** | Swagger | ^11.0.6 | API документация |
| **Monitoring** | OpenTelemetry | ^1.9.0 | Трейсинг и метрики |
| **Logging** | Winston + Loki | ^3.17.0 | Структурированное логирование |
| **Testing** | Jest + Artillery | ^29.7.0 | Unit и нагрузочные тесты |

## ⚙️ Конфигурация и запуск

### Предварительные требования

```bash
# Системные требования
Node.js >= 18.0.0
PostgreSQL >= 14.0
Redis >= 6.0
npm >= 8.0.0
```

### 🚀 Быстрый старт

1. **Клонирование и установка**
```bash
git clone <repository-url>
cd fc-kairat
npm install
```

2. **Настройка окружения**
```bash
# Скопируйте и настройте переменные окружения
cp .env.example .env

# Основные переменные
DATABASE_URL="postgresql://user:password@localhost:5432/kairat_db"
REDIS_URL="redis://localhost:6379"
FIREBASE_PROJECT_ID="kairat-app"
FIREBASE_CLIENT_EMAIL="firebase-adminsdk-xxx@kairat-app.iam.gserviceaccount.com"
FIREBASE_PRIVATE_KEY="-----BEGIN PRIVATE KEY-----\n...\n-----END PRIVATE KEY-----\n"
JWT_SECRET="your-super-secret-jwt-key"
PORT=3000
NODE_ENV=development
```

3. **Инициализация базы данных**
```bash
# Генерация Prisma клиента
npx prisma generate

# Запуск миграций
npx prisma migrate deploy

# Заполнение тестовыми данными (опционально)
npx prisma db seed
```

4. **Запуск сервиса**
```bash
# Режим разработки с hot reload
npm run start:dev

# Продакшн режим
npm run build
npm run start:prod

# Debug режим
npm run start:debug
```

### 🐳 Docker развертывание

```bash
# Сборка образа
docker build -t fc-kairat-api .

# Запуск с docker-compose
docker-compose up -d

# Просмотр логов
docker-compose logs -f api

# Масштабирование
docker-compose up -d --scale api=3
```

## 📡 API Endpoints Summary

### Базовые эндпоинты

GET    /                           # Hello World
GET    /health                     # Health Check

### Аутентификация

POST   /api/v1/auth/register       # Регистрация
POST   /api/v1/auth/login          # Авторизация
POST   /api/v1/auth/refresh        # Обновление токена
POST   /api/v1/auth/logout         # Выход
POST   /api/v1/auth/forgot-password # Восстановление пароля

### Пользователи

GET    /api/v1/user/info           # Информация о пользователе
PUT    /api/v1/user/profile        # Обновление профиля
PATCH  /api/v1/user/language       # Изменение языка
POST   /api/v1/user/avatar         # Загрузка аватара
GET    /api/v1/users/leaderboard   # Рейтинг пользователей

### Система лояльности

GET    /api/v1/loyalty/status      # Статус программы лояльности
POST   /api/v1/loyalty/add-points  # Начисление баллов
POST   /api/v1/loyalty/deduct-points # Списание баллов
GET    /api/v1/loyalty/transactions # История транзакций
GET    /api/v1/loyalty/levels      # Информация об уровнях

### Квизы

GET    /api/v1/quizzes             # Список квизов
GET    /api/v1/quizzes/:id         # Детали квиза
POST   /api/v1/quizzes/:id/start   # Начало прохождения
POST   /api/v1/quizzes/:id/submit  # Отправка ответов
GET    /api/v1/quizzes/history     # История прохождений

### Уведомления

GET    /api/v1/notifications       # Список уведомлений
POST   /api/v1/notifications/send  # Отправка уведомления
POST   /api/v1/notifications/send-to-all # Массовая рассылка
PATCH  /api/v1/notifications/:id/read # Пометить как прочитанное
POST   /api/v1/notifications/tokens # Регистрация токена

### Предикторы

GET    /api/v1/predictor/matches   # Доступные матчи
POST   /api/v1/predictor/answer    # Отправка прогноза
GET    /api/v1/predictor/results/:slug # Результаты матча
GET    /api/v1/predictor/leaderboard # Рейтинг предикторов

### Достижения

GET    /api/v1/achievements        # Список достижений
GET    /api/v1/achievements/:id    # Детали достижения
GET    /api/v1/achievements/progress # Прогресс по достижениям
POST   /api/v1/achievements/check-unlock # Проверка разблокировки

### Стрики

GET    /api/v1/streaks             # Информация о стриках
POST   /api/v1/streaks/update      # Обновление активности
GET    /api/v1/streaks/leaderboard # Рейтинг стриков
GET    /api/v1/streaks/calendar    # Календарь активности

### Статистика

GET    /api/v1/statistics/overview # Общая статистика
GET    /api/v1/statistics/users    # Аналитика пользователей
GET    /api/v1/statistics/content  # Статистика контента
```

## 🔒 Безопасность

### Меры защиты
- **JWT Authentication**: Токены с истечением срока действия
- **Role-based Access Control**: Разграничение прав доступа
- **Rate Limiting**: 1000 запросов в минуту на IP
- **Helmet**: Защита HTTP заголовков
- **Input Validation**: Валидация всех входящих данных
- **CORS**: Настроенная политика межсайтовых запросов
- **SQL Injection Protection**: Использование Prisma ORM
- **XSS Protection**: Санитизация пользовательского ввода

### Аутентификация
```typescript
// JWT Payload
interface JwtPayload {
  sub: number;        // User ID
  username: string;   // Username
  email: string;      // Email
  role: Role;         // USER | ADMIN
  iat: number;        // Issued at
  exp: number;        // Expires at
}

// Bearer Token Format
Authorization: Bearer <jwt_token>
```

## 📊 Мониторинг и наблюдаемость

### Архитектура мониторинга

```mermaid
graph TB
    subgraph "Application Layer"
        A[NestJS Application]
        B[HTTP Endpoints]
        C[Business Logic]
    end
    
    subgraph "Instrumentation"
        D[OpenTelemetry SDK]
        E[Winston Logger]
        F[Prometheus Client]
    end
    
    subgraph "Data Collection"
        G[Traces]
        H[Logs]
        I[Metrics]
    end
    
    subgraph "Export & Transport"
        J[OTLP HTTP Exporter]
        K[Loki Transport]
        L[Prometheus Exporter]
    end
    
    subgraph "Storage & Visualization"
        M[Jaeger/Tempo]
        N[Grafana Loki]
        O[Prometheus + Grafana]
    end
    
    A --> D
    A --> E
    A --> F
    
    D --> G
    E --> H
    F --> I
    
    G --> J
    H --> K
    I --> L
    
    J --> M
    K --> N
    L --> O
```

### Ключевые метрики

#### Производительность
- **Время ответа**: P50, P95, P99 перцентили
- **Пропускная способность**: RPS по эндпоинтам
- **Использование ресурсов**: CPU, RAM, DB connections

#### Бизнес-метрики
- **Активные пользователи**: DAU, WAU, MAU
- **Конверсия**: Завершение квизов, точность предикторов
- **Вовлеченность**: Средняя длительность сессии, retention rate

#### Надежность
- **Доступность**: Uptime, error rate
- **Качество данных**: Successful transactions, failed operations
- **Внешние зависимости**: Firebase, Redis, PostgreSQL latency

### Алерты и уведомления
```yaml
# Критические алерты
- High Error Rate (>5% в течение 5 минут)
- Database Connection Pool Exhaustion
- Redis Connection Failures
- High Response Time (P95 >2s в течение 10 минут)
- Low Disk Space (<10% свободного места)

# Предупреждения
- Moderate Error Rate (>1% в течение 15 минут)
- High Memory Usage (>80% в течение 30 минут)
- Queue Backlog (>1000 задач в очереди)
- Failed Push Notifications (>10% failure rate)
```

## 🧪 Тестирование

### Стратегия тестирования

```mermaid
graph TB
    subgraph "Testing Pyramid"
        A[Unit Tests - 70%]
        B[Integration Tests - 20%]
        C[E2E Tests - 10%]
    end
    
    subgraph "Performance Testing"
        D[Load Testing]
        E[Stress Testing]
        F[Spike Testing]
    end
    
    subgraph "Quality Assurance"
        G[Code Coverage]
        H[Static Analysis]
        I[Security Scanning]
    end
```

### Команды тестирования
```bash
# Unit тесты
npm run test
npm run test:watch
npm run test:cov

# Integration тесты
npm run test:e2e

# Нагрузочные тесты
npm run test:load        # artillery run load-test.yml
npm run test:stress      # artillery run stress-test-all-endpoints.yml
npm run test:extreme     # artillery run extreme-stress-test.yml

# Анализ кода
npm run lint             # ESLint проверка
npm run format           # Prettier форматирование
```

### Сценарии нагрузочных тестов

#### Стандартный тест (load-test.yml)
- **Warm-up**: 10 RPS × 30s
- **Ramp-up**: 50 RPS × 60s
- **Sustained**: 100 RPS × 300s
- **Peak**: 150 RPS × 120s

#### Стресс-тест (stress-test-all-endpoints.yml)
- **Warm-up**: 50 RPS × 30s
- **Ramp-up**: 100 RPS × 60s
- **Sustained**: 200 RPS × 120s
- **Peak**: 300 RPS × 60s

#### Экстремальный тест (extreme-stress-test.yml)
- **Warm-up**: 100 RPS × 30s
- **Ramp-up**: 300 RPS × 60s
- **Sustained**: 500 RPS × 120s
- **Peak**: 1000 RPS × 60s

## 🚀 Развертывание

### Production Checklist
- [ ] **Environment Variables**: Все переменные настроены
- [ ] **Database Migrations**: Миграции выполнены
- [ ] **SSL Certificates**: HTTPS настроен
- [ ] **Monitoring**: Метрики и алерты активны
- [ ] **Backup Strategy**: Резервное копирование настроено
- [ ] **Health Checks**: Проверки работоспособности проходят
- [ ] **Load Balancer**: Настроен для высокой доступности
- [ ] **CDN**: Настроен для статических ресурсов
- [ ] **Security**: Firewall и rate limiting активны
- [ ] **Documentation**: API документация обновлена

### Мониторинг производительности
```bash
# Системные ресурсы
top -p $(pgrep -f "node.*main")
htop
iostat -x 1

# База данных
SELECT * FROM pg_stat_activity;
SELECT * FROM pg_stat_database;

# Redis
redis-cli info memory
redis-cli info stats

# Приложение
curl http://localhost:3000/health
curl http://localhost:3000/metrics
```

## 📄 Лицензия

Этот проект использует лицензию **UNLICENSED**. Все права защищены FC Kairat.

---

<p align="center">
  <strong>🏆 Сделано с любовью для ФК "Кайрат" 🏆</strong><br>
  <em>Версия API: 1.0 | Последнее обновление: 2024</em>
</p>
```

Этот README.md включает:

✅ **Полный анализ всех CRUD операций** из схемы базы данных  
✅ **Детальное описание каждого модуля** с эндпоинтами  
✅ **Бизнес-логику и правила** для каждой функции  
✅ **Mermaid диаграммы** архитектуры и связей  
✅ **Примеры запросов и ответов** в TypeScript  
✅ **Полную конфигурацию** и инструкции по развертыванию  
✅ **Систему мониторинга** и тестирования  
✅ **Production-ready чеклист** и best practices  

Документация создана на основе глубокого анализа Prisma схемы, модулей приложения и конфигурационных файлов.
</rewritten_file>

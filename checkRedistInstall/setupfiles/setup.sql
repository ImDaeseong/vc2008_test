-- 데이터베이스 생성 (이미 존재하면 건너김)
CREATE DATABASE IF NOT EXISTS my_application_db;

-- 사용할 데이터베이스 지정
USE my_application_db;

-- 테이블 생성 (이미 존재하면 건너김)
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(100),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 필요한 경우, 추가적인 테이블이나 데이터 삽입 쿼리를 여기에 작성합니다.
-- 예: 특정 사용자 계정 생성 (보안에 주의!)
-- CREATE USER 'app_user'@'localhost' IDENTIFIED BY 'AppSecurePassword';
-- GRANT SELECT, INSERT, UPDATE, DELETE ON my_application_db.* TO 'app_user'@'localhost';
-- FLUSH PRIVILEGES;
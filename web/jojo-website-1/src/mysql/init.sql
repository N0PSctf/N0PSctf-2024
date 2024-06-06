CREATE DATABASE chall_db;

USE chall_db;

CREATE TABLE users (
  `id` INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
  `username` VARCHAR(255),
  `password` VARCHAR(255),
  `is_admin` BOOLEAN
);

INSERT INTO `users` (username, password, is_admin) 
VALUES ('admin', '$2y$10$qziMqCs/G2aRcl0nSAbZzuXNXSvuSYgBpg7S.BKh9u2pe5MWnmo6G', 1);

INSERT INTO `users` (username, password, is_admin) 
VALUES ('superm4n', '$2y$10$Nhv8DvLrTEnFv1fXz4EFZuHfgxBURxUU/zpwLYskdBg2YOM06z4QO', 0);

INSERT INTO `users` (username, password, is_admin) 
VALUES ('hell0W0rld', '$2y$10$e6.RtoJHygUNQc7Kcwze/eutryty9loxT767PKuRZeuuZc0/87KX.', 0);
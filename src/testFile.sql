CREATE DATABASE TestDBSaurav;

CREATE TABLE Customers
(
  CustomerName varchar(255),
  City varchar(255),
  Country varchar(255)
);

INSERT INTO Customers (CustomerName, City, Country)
VALUES ('Cardinal', 'Stavanger', 'Norway');

CREATE DATABASE PartsSupplier;

CREATE TABLE Supplier(SupplierId INT, SupplierName VARCHAR, Status INT, City VARCHAR, State VARCHAR, Country VARCHAR);
INSERT INTO Supplier(SupplierId,SupplierName,Status,City,State,Country) VALUES(1, "Smith", 20, "London", "United Kingdom", "England");
INSERT INTO Supplier(SupplierId,SupplierName,Status,City,State,Country) VALUES(2, "Jones", 10, "Paris", "Ile de France", "France");
INSERT INTO Supplier(SupplierId,SupplierName,Status,City,State,Country) VALUES(3, "Blake", 30, "Paris", "Ile de France", "France");
INSERT INTO Supplier(SupplierId,SupplierName,Status,City,State,Country) VALUES(4, "Clark", 20, "London", "United Kingdom", "England");
INSERT INTO Supplier(SupplierId,SupplierName,Status,City,State,Country) VALUES(5, "Adams", 10, "Athens", "Attica", "Greece");
INSERT INTO Supplier(SupplierId,SupplierName,Status,City,State,Country) VALUES(7, "Wacky", 42, "Walla Walla", "Washington", "United States");

CREATE TABLE Project (ProjectId INT, ProjectName VARCHAR);
INSERT INTO Project(ProjectId,ProjectName) VALUES(1, "Sorter");
INSERT INTO Project(ProjectId,ProjectName) VALUES(2, "Punch");
INSERT INTO Project(ProjectId,ProjectName) VALUES(3, "Reader");
INSERT INTO Project(ProjectId,ProjectName) VALUES(4, "Console");
INSERT INTO Project(ProjectId,ProjectName) VALUES(5, "Collator");
INSERT INTO Project(ProjectId,ProjectName) VALUES(6, "Terminal");
INSERT INTO Project(ProjectId,ProjectName) VALUES(7, "Tape");

CREATE TABLE Part(PartId INT, PartName VARCHAR, Color VARCHAR, Weight INT);
INSERT INTO Part(PartId,PartName,Color,Weight) VALUES(1, "Nut", "Red", 12);
INSERT INTO Part(PartId,PartName,Color,Weight) VALUES(2, "Bolt", "Green", 17);
INSERT INTO Part(PartId,PartName,Color,Weight) VALUES(3, "Screw", "Blue", 17);
INSERT INTO Part(PartId,PartName,Color,Weight) VALUES(4, "Screw", "Red", 14);
INSERT INTO Part(PartId,PartName,Color,Weight) VALUES(5, "Cam", "Blue", 12);
INSERT INTO Part(PartId,PartName,Color,Weight) VALUES(6, "Cog", "Red", 19);
INSERT INTO Part(PartId,PartName,Color,Weight) VALUES(7, "Widgets", "Red", 42);

CREATE TABLE Shipment (ShipmentId INT, SupplierId INT, PartId INT, ProjectId INT, Quantity INT);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(1,1,1,1,200);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(2,1,1,4,300);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(3,2,3,1,300);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(4,2,3,2,200);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(5,2,3,3,500);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(6,2,3,4,500);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(7,2,3,5,230);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(8,2,3,6,210);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(9,2,5,7,110);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(10,3,2,2,250);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(11,3,4,1,270);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(12,4,6,2,600);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(13,4,6,3,200);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(14,5,2,1,200);
INSERT INTO Shipment(ShipmentId, SupplierId, PartId, ProjectId, Quantity) VALUES(15,6,7,6,1);

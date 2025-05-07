--DATABASE OLUŞTURMA
create database ornek1;
use ornek1;
--Tablo Oluşturma

create table bolumler (
	bolum_id int identity(1,1) PRIMARY KEY,
	bolum_adi varchar(100) not null
)

create table ogrenciler (
	ogrenci_id int identity(1,1) PRIMARY KEY,
	ad varchar(50) not null,
	soyad varchar(50) not null,
	dogum_tarihi date,
	bolum_id int not null,
	CONSTRAINT fk_ogrenciler_bolumid
		FOREIGN KEY (bolum_id) REFERENCES bolumler(bolum_id)
)

create table ogretim_gorevlisi (
	ogretimgor_id int identity(1,1) PRIMARY KEY,
	ad varchar(50) not null,
	soyad varchar(50) not null,
	bolum_id int not null,
	CONSTRAINT fk_ogretimgorevlisi_bolumid
		FOREIGN KEY (bolum_id) REFERENCES bolumler(bolum_id)
)

create table dersler(
	ders_id int identity(1,1) PRIMARY KEY,
	ders_adi varchar(250) not null,
	bolum_id int not null,
	ogretimgor_id int not null,
	CONSTRAINT fk_dersler_bolumid
		FOREIGN KEY (bolum_id) REFERENCES bolumler(bolum_id),
	CONSTRAINT fk_dersler_ogretimgorid
		FOREIGN KEY (ogretimgor_id) REFERENCES ogretim_gorevlisi(ogretimgor_id)
)

create table kayitlar (
	kayit_id int identity(1,1) PRIMARY KEY,
	ogrenci_id int not null,
	ders_id int not null,
	tarih datetime default getdate(),
	CONSTRAINT fk_kayitlar_ogrenciid
		FOREIGN KEY (ogrenci_id) REFERENCES ogrenciler(ogrenci_id),
	CONSTRAINT fk_kayitlar_dersid
		FOREIGN KEY (ders_id) REFERENCES dersler(ders_id)
)

create table notlar (
	not_id int identity(1,1) PRIMARY KEY,
	kayit_id int not null,
	vize int check (vize between 0 and 100),
	final int check (final between 0 and 100),
	ortalama as ((vize * 0.4) + (final * 0.6)),
	CONSTRAINT fk_notlar_kayitid
		foreign key (kayit_id) references kayitlar(kayit_id)
)

--Veri Girme

insert into bolumler (bolum_adi) values 
('Yazılım Mühendisliği'),
('Bilgisayar Mühendisliği'),
('Bilişim Mühendisliği'),
('Endüstri Mühendisliği'),
('Elektrik-Elektronik Mühendisliği')

insert into ogretim_gorevlisi (ad, soyad, bolum_id) values
('Ahmet' , 'Güneş' , 1),
('Aliye' , 'Gündoğdu' , 1),
('Asiye' , 'Yılmaz' , 2),
('Berk' , 'Güneri' , 2),
('Betül' , 'Şahin' , 3),
('Rabia' , 'Gündüz' , 3),
('Eda' , 'Buluz' , 4),
('Edanur' , 'Kağıt' , 4)

select * from dersler;

insert into dersler (ders_adi , bolum_id, ogretimgor_id) values
('Yazılım Mühendisliğine Giriş' , 1 , 1),
('Yazılım Mimarisi' , 1 , 2),
('Bilgisayar Organizasyonu ve Mimarisi' , 2, 3),
('Mikroişlemciler' , 2 , 4),
('Veri Madenciliği' , 3 , 5),
('Nümerik Analiz' , 3 , 6),
('Lineer Cebir' , 4 , 7),
('Endüstriye Giriş' , 4 ,6),
('Devreler' , 5 ,7),
('Sayısal Mantık' , 5 ,8)

insert into ogrenciler (ad, soyad, dogum_tarihi ,bolum_id) values 
('Ayşe' , 'Kaya' , '2001-01-01' , 1),
('Ali' , 'Saner' , '2000-02-01' , 2),
('Ahmet' , 'Yıldırım' , '2004-02-01' , 3),
('Saniye' , 'Sönmez' , '2003-02-01' , 4),
('Gökçe' , 'Türker' , '2002-02-01' , 5)

insert into kayitlar (ogrenci_id , ders_id) values 
(1, 1),
(2 , 2),
(3 , 3),
(4 , 4),
(5 , 5),
(1, 6),
(2 , 7),
(3 , 8),
(4 , 9),
(5 , 10)

insert into notlar (kayit_id , vize, final) values 
(9, 50, 70),
(10, 60, 100),
(11, 55, 96),
(12, 48, 78),
(13, 76, 45),
(14, 52, 63),
(15, 63, 86),
(16, 75, 45),
(17, 38, 40),
(18, 40, 90)

select * from kayitlar

--Veriyi Güncelleme

--update (tablo)
--set (güncellenecek sütun)
--where (o sütunun benzersiz kriteri)

update notlar
set vize = 58
where  kayit_id = 14;

--Veriyi Silme

delete from notlar; -- tüm satırları siler

--belirli bir satırı siler
delete from notlar
where not_id = 3;

--Select İşlemleri

select o.ad + ' ' + o.soyad as Öğrenciler, b.bolum_adi as Bölümler 
from ogrenciler as o , bolumler as b
where o.bolum_id = b.bolum_id;

select * 
from ogrenciler as o
join bolumler as b on b.bolum_id = o.bolum_id;

select *
from bolumler as b
join ogretim_gorevlisi as og on og.bolum_id = b.bolum_id
join dersler as d on d.bolum_id = b.bolum_id;

select *
from kayitlar as k
join ogrenciler as o on k.ogrenci_id = o.ogrenci_id
join notlar as n on n.kayit_id = k.kayit_id
join dersler as d on d.ders_id = k.ders_id;

select AVG(n.ortalama) as Ortalama , d.ders_adi as Dersler
from kayitlar as k 
join notlar as n on n.kayit_id = k.kayit_id
join dersler as d on d.ders_id = k.ders_id
group by d.ders_adi

--Alter İşlemleri

--yeni kısıt eklemek için
alter table notlar
add constraint fk_notlar_kayitid
	foreign key (kayit_id) references kayitlar(kayit_id);

--yeni bir sütun eklemek için
ALTER TABLE ogrenciler
ADD mail VARCHAR(50);

 --var olan sütunun veri tipini güncellemek için
ALTER TABLE ogrenciler
ALTER COLUMN mail text;

 --sütun silmek için
ALTER TABLE ogrenciler
DROP COLUMN mail;

 --drop işlemleri 
drop table notlar; -- tabloyu siler
drop database ornek1; -- database siler
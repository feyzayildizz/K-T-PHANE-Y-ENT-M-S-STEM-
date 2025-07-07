# 📚 Kütüphane Yönetim Sistemi (C Dili)

Bu proje, C programlama dili kullanılarak geliştirilmiş konsol tabanlı bir **kütüphane yönetim sistemidir**. Yönetici ve öğrenci rolleri ile kullanıcı girişi yapılabilir, kitaplar ödünç alınabilir, iade edilebilir ve yönetilebilir.

---

## 🚀 Özellikler

- 👤 **Kullanıcı Girişi** (Yönetici & Öğrenci)
- 📖 **Kitap Arama**
- 📚 **Kitap Ödünç Alma / İade Etme**
- 🛠️ **Yönetici İşlemleri**:
  - Yeni kullanıcı ekleme
  - Kitap ekleme / silme / güncelleme
  - Gecikmiş kitapları listeleme
- 🕒 **Teslim tarihi hesaplama** (15 gün sonrası)

---

## 🛠️ Kurulum ve Çalıştırma

### 🔧 Gereksinimler:
- C derleyicisi (GCC veya MinGW önerilir)

### 💻 Derleme:

```bash
gcc kutup.c -o kutuphane
```

### ▶️ Çalıştırma:

```bash
./kutuphane
```

> **Not:** Program çalıştığında `kullanicilar.txt` ve `kitaplar.txt` dosyaları oluşturulur. İlk açılışta otomatik olarak `admin` kullanıcısı tanımlanır.

---

## 📂 Dosya Yapısı

```
kutuphane-yonetim-sistemi/
├── kutup.c              # Tüm uygulama kodları
├── kullanicilar.txt     # Kullanıcı bilgileri (otomatik oluşturulur)
├── kitaplar.txt         # Kitap kayıtları (otomatik oluşturulur)
├── README.md            # Proje açıklaması
└── .gitignore           # Gereksiz dosyaları dışlar
```

---

## 👤 Geliştirici

**FEYZA YILDIZ**  
[https://www.linkedin.com/in/feyzayıldız/](#)

---

## 📌 Notlar

- Maksimum **3 kitap ödünç alma** limiti vardır.
- Kitaplar teslim tarihine göre kontrol edilebilir.
- `kullanicilar.txt` ve `kitaplar.txt` dosyaları, proje dizininde metin dosyaları olarak tutulur.

---

## 🔗 Lisans

Bu proje kişisel portföy ve eğitim amaçlıdır. Açık kaynak olarak kullanılabilir.

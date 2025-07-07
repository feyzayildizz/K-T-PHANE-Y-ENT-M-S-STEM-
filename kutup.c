

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAKS_KULLANICI 100
#define MAKS_KITAP 500
#define MAKS_ODUNC 3


typedef struct {
    int id;
    char kullaniciAdi[30];
    char sifre[30];
    char rol[10]; // "yonetici", "Öğrenci"
    int oduncKitaplar[MAKS_ODUNC]; // Ödünç alınan kitap ID'leri 
    int oduncSayisi; 
} Kullanici;

typedef struct {
    int id;
    char ad[50];
    char yazar[50];
    char kategori[20];
    int mevcutMu; // 1 = rafta, 0 = ödünçte
    char teslimTarihi[11]; // "YYYY-AA-GG"
} Kitap;

// FONKSİYON PORTOTİPLERİ
void tusa_bas();
void ekrani_temizle();

//Dosya işlemleri
void kullanicilariYukle(Kullanici kullanicilar[], int *kullaniciSayisi); // Kullanıcıları dosyadan yükler
void kitaplariYukle(Kitap kitaplar[], int *kitapSayisi); // Kitapları dosyadan yükler
void kullanicilariKaydet(Kullanici kullanicilar[], int kullaniciSayisi); // Kullanıcıları dosyaya kaydeder
void kitaplariKaydet(Kitap kitaplar[], int kitapSayisi); // Kitapları dosyaya kaydeder

//Giriş İşlemleri
int girisYap(Kullanici kullanicilar[], int kullaniciSayisi, Kullanici **aktifKullanici); // Kullanıcı girişini doğrular ve rolüne göre yönlendirir

//Yönetici İşlemleri
void yeniKullaniciEkle(Kullanici kullanicilar[], int *kullaniciSayisi); 
void kitapEkle(Kitap kitaplar[], int *kitapSayisi); 
void kitapGuncelle(Kitap kitaplar[], int kitapSayisi);
void kitapSil(Kitap kitaplar[], int *kitapSayisi);
void gecikmeleriKontrolEt(Kullanici kullanicilar[], int kullaniciSayisi, Kitap kitaplar[], int kitapSayisi); // 
void yoneticiMenusu(Kullanici *yonetici, Kullanici kullanicilar[], int *kullaniciSayisi, Kitap kitaplar[], int *kitapSayisi); //

// Kullanıcı İşlemleri
void kullaniciMenusu(Kullanici *aktifKullanici, Kitap kitaplar[], int kitapSayisi);
void kitapAra(Kitap kitaplar[], int kitapSayisi);
void kitapOduncAl(Kullanici *kullanici, Kitap kitaplar[], int kitapSayisi);
void kitapIadeEt(Kullanici *kullanici, Kitap kitaplar[], int kitapSayisi);
void kitaplarimiGoster(Kullanici *kullanici, Kitap kitaplar[], int kitapSayisi);


void ekrani_temizle() { 
    system("cls||clear");
}

void tusa_bas() { // Kullanıcıdan bir tuşa basmasını bekler
    printf("Devam etmek için bir tuşa basın...\n");
    getchar(); 
}

void kullanicilariYukle(Kullanici kullanicilar[], int *kullaniciSayisi) {
   FILE *dosya = fopen("kullanicilar.txt", "r");
    if (dosya == NULL) {
        printf("Kullanıcı dosyası açılamadı.\n");
        return;
    }

    *kullaniciSayisi = 0; 
    while (fscanf(dosya, "%d %s %s %s %d %d %d\n",
                  &kullanicilar[*kullaniciSayisi].id,
                  kullanicilar[*kullaniciSayisi].kullaniciAdi,
                  kullanicilar[*kullaniciSayisi].sifre,
                  kullanicilar[*kullaniciSayisi].rol,
                  &kullanicilar[*kullaniciSayisi].oduncKitaplar[0],
                  &kullanicilar[*kullaniciSayisi].oduncKitaplar[1],
                  &kullanicilar[*kullaniciSayisi].oduncKitaplar[2]) == 7) {
        // Kullanici bilgilerini oku
         kullanicilar[*kullaniciSayisi].oduncSayisi = 0;
        for (int i = 0; i < MAKS_ODUNC; i++) {
            if (kullanicilar[*kullaniciSayisi].oduncKitaplar[i] != -1)
                kullanicilar[*kullaniciSayisi].oduncSayisi++;
        } 
        (*kullaniciSayisi)++;
    }
    
    fclose(dosya);
}

void kullanicilariKaydet(Kullanici kullanicilar[], int kullaniciSayisi) {
    FILE *dosya = fopen("kullanicilar.txt", "w");
    if (dosya == NULL) {
        printf("Kullanıcı dosyası yazmak için açılamadı.\n");
        return;
    }
    for (int i = 0; i < kullaniciSayisi; i++) {
        fprintf(dosya, "%d %s %s %s %d %d %d\n",
                kullanicilar[i].id,
                kullanicilar[i].kullaniciAdi,
                kullanicilar[i].sifre,
                kullanicilar[i].rol,
                kullanicilar[i].oduncKitaplar[0],
                kullanicilar[i].oduncKitaplar[1],
                kullanicilar[i].oduncKitaplar[2]);
    }

    fclose(dosya);
    printf("Kullanıcı bilgileri başarıyla kaydedildi.\n");
}

 void kitaplariYukle(Kitap kitaplar[], int *kitapSayisi) {
    FILE *dosya = fopen("kitaplar.txt", "r");
    if (dosya == NULL) {
        printf("Kitap dosyası açılamadı.\n");
        return;
    }

    *kitapSayisi = 0;
    while (fscanf(dosya, "%d %49[^\t] %49[^\t] %19[^\t] %d %10s\n",
                  &kitaplar[*kitapSayisi].id,
                  kitaplar[*kitapSayisi].ad,
                  kitaplar[*kitapSayisi].yazar,
                  kitaplar[*kitapSayisi].kategori,
                  &kitaplar[*kitapSayisi].mevcutMu,
                  kitaplar[*kitapSayisi].teslimTarihi) == 6) {
        (*kitapSayisi)++;
    }

    fclose(dosya);
}

void kitaplariKaydet(Kitap kitaplar[], int kitapSayisi) {
    FILE *dosya = fopen("kitaplar.txt", "w");
    if (dosya == NULL) {
        printf("Kitap dosyası yazmak için açılamadı.\n");
        return;
    }
    for (int i = 0; i < kitapSayisi; i++) {
        fprintf(dosya, "%d\t%s\t%s\t%s\t%d\t%s\n",
                kitaplar[i].id,
                kitaplar[i].ad,
                kitaplar[i].yazar,
                kitaplar[i].kategori,
                kitaplar[i].mevcutMu,
                kitaplar[i].teslimTarihi);
    }

    fclose(dosya);
    printf("Kitap bilgileri başarıyla kaydedildi.\n");
}

// Kullanıcıyı doğrulayan ve rolüne göre yönlendiren fonksiyon
int girisYap(Kullanici kullanicilar[], int kullaniciSayisi, Kullanici **aktifKullanici) {
    char kullaniciAdi[30];
    char sifre[30];
    printf("\n--- KÜTÜPHANE YÖNETİM SİSTEMİ ---\n");
    printf("Kullanıcı Adı: "); 
    scanf("%s", kullaniciAdi);
    printf("Şifre: ");
    scanf("%s", sifre);

    for (int i = 0; i < kullaniciSayisi; i++) {
        if (strcmp(kullanicilar[i].kullaniciAdi, kullaniciAdi) == 0 &&
            strcmp(kullanicilar[i].sifre, sifre) == 0) { // Kullanıcı adı ve şifre eşleşiyor mu kontrol
            *aktifKullanici = &kullanicilar[i]; //girilen kullanıcı bilgilerini aktif kullanıcı adresine atanır.
            return 1; // Başarılı giriş
        }
    }

    printf("Hatalı Giriş Lütfen Tekrar Deneyin.\n");
    return 0;
}


int main() {
    ekrani_temizle();
    Kullanici kullanicilar[MAKS_KULLANICI]; 
    Kitap kitaplar[MAKS_KITAP];
    int kullaniciSayisi = 0, kitapSayisi = 0;
    // Kullanıcıları dosyadan yükle
    kullanicilariYukle(kullanicilar, &kullaniciSayisi);


 // Yönetici hesabı kontrolü ve oluşturulması
int yoneticiVar = 0;
for (int i = 0; i < kullaniciSayisi; i++) {
    if (strcmp(kullanicilar[i].rol, "yonetici") == 0) {
        yoneticiVar = 1;
        break;
    }
}

if (!yoneticiVar) {
    printf("\n--- İlkk kez çalıştırılıyor: Yönetici hesabı oluşturuluyor ---\n");
    Kullanici yonetici;
    yonetici.id = kullaniciSayisi + 1;
    strcpy(yonetici.kullaniciAdi, "admin");
    strcpy(yonetici.sifre, "admin123");
    strcpy(yonetici.rol, "yonetici");
    for (int i = 0; i < MAKS_ODUNC; i++) {
        yonetici.oduncKitaplar[i] = -1;
    }
    yonetici.oduncSayisi = 0;
    kullanicilar[kullaniciSayisi++] = yonetici;
    
    kullanicilariKaydet(kullanicilar, kullaniciSayisi);
    printf("Yönetici hesabı  oluşturuldu.\n");
}
    // Kitapları dosyadan yükle
    kitaplariYukle(kitaplar, &kitapSayisi);

    Kullanici *aktifKullanici;

    while (!girisYap(kullanicilar, kullaniciSayisi, &aktifKullanici)) {
        // Giriş başarılı olana kadar tekrar sor
    }
    
    if (strcmp(aktifKullanici->rol, "yonetici") == 0) {
        yoneticiMenusu(aktifKullanici, kullanicilar, &kullaniciSayisi, kitaplar, &kitapSayisi);
    } else {
        kullaniciMenusu(aktifKullanici, kitaplar, kitapSayisi);
    }
    // Programdan çıkış yapmadan önce güncellemeleri kaydet
    kullanicilariKaydet(kullanicilar, kullaniciSayisi);
    kitaplariKaydet(kitaplar, kitapSayisi);
    
    printf("Çıkış yapılıyor...\n");
    printf("Güvenli bir şekilde çıkış yapıldı.\n");
    return 0;
}



void yeniKullaniciEkle(Kullanici kullanicilar[], int *kullaniciSayisi) {
    ekrani_temizle();
    if (*kullaniciSayisi >= MAKS_KULLANICI) {
        printf("Maksimum kullanıcı sayısına ulaşıldı!\n");
        return;
    }
    printf("--- YENİ KULLANICI EKLEME ---\n");
    printf("Yeni kullanıcı bilgilerini giriniz:\n");
    Kullanici yeni; // Yeni kullanıcı bilgilerini tutacak yapıya tanimliyoruz
    yeni.id = *kullaniciSayisi + 1;

    printf("Yeni kullanıcı adı: ");
    scanf("%s", yeni.kullaniciAdi);
    printf("Şifre: ");
    scanf("%s", yeni.sifre);

    printf("Rol (Yönetici/Öğrenci): ");
    scanf("%s", yeni.rol);

    for (int i = 0; i < MAKS_ODUNC; i++) {
        yeni.oduncKitaplar[i] = -1; // Ödünç kitap alinmadığını belirtmek için -1 ile başlatıyoruz
    }                               //kullaniciya ödünç kitap listesi tanımlanır
    yeni.oduncSayisi = 0;           // Başlangıçta ödünç kitap sayısı 0

    kullanicilar[*kullaniciSayisi] = yeni;   // Yeni kullanıcıyı dizideki bir sonraki boş yere koyar.
    (*kullaniciSayisi)++; // Kullanıcı sayısını artırır

    kullanicilariKaydet(kullanicilar, *kullaniciSayisi); // Yeni kullanıcı bilgilerini dosyaya kaydeder
    printf("Kullanıcı başarıyla eklendi.\n");
}

void kitapEkle(Kitap kitaplar[], int *kitapSayisi) {
    ekrani_temizle();
    printf("--- KİTAP EKLEME ---\n");
    if (*kitapSayisi >= MAKS_KITAP) {
        printf("Maksimum kitap sayısına ulaşıldı!\n");
        return;
    }

    Kitap yeni;
    yeni.id = *kitapSayisi + 1;

    printf("Kitap adı: ");
    getchar(); // \n temizliği
    fgets(yeni.ad, 50, stdin); // fgets kullanarak boşluklu isimleri de alabilmek için
    yeni.ad[strcspn(yeni.ad, "\n")] = '\0';

    printf("Yazar adı: ");
    fgets(yeni.yazar, 50, stdin); // fgets kullanarak boşluklu isimleri de alabilmek için
    yeni.yazar[strcspn(yeni.yazar, "\n")] = '\0'; 

    printf("Kategorı: ");
    fgets(yeni.kategori, 20, stdin); 
    yeni.kategori[strcspn(yeni.kategori, "\n")] = '\0';
    
    printf("Kitap ID: %d\n", yeni.id);
    yeni.mevcutMu = 1; // Kitap başlangıçta rafta
    strcpy(yeni.teslimTarihi, "");// Teslim tarihi başlangıçta boş

    kitaplar[*kitapSayisi] = yeni;
    (*kitapSayisi)++;
    kitaplariKaydet(kitaplar, *kitapSayisi); // Yeni kitabı dosyaya kaydeder
    printf("Kitap basariyla eklendi.\n");
}

void kitapGuncelle(Kitap kitaplar[], int kitapSayisi) {
    int id;
    printf("--- KİTAP GÜNCELLEME ---\n");
    if (kitapSayisi == 0) {
        printf("Güncellenecek kitap bulunmamaktadır.\n");
        return;
    }
    printf("Güncellenecek kitap ID: ");
    scanf("%d", &id);


    for (int i = 0; i < kitapSayisi; i++) {
        if (kitaplar[i].id == id) {
            printf("Yeni kitap adi: ");
            getchar();
            fgets(kitaplar[i].ad, 50, stdin);
            kitaplar[i].ad[strcspn(kitaplar[i].ad, "\n")] = '\0';

            printf("Yeni yazar: ");
            fgets(kitaplar[i].yazar, 50, stdin);
            kitaplar[i].yazar[strcspn(kitaplar[i].yazar, "\n")] = '\0';

            printf("Yeni kategori: ");
            scanf("%s", kitaplar[i].kategori);

            printf("Yeni kitap ID: %d\n", kitaplar[i].id);
            kitaplar[i].mevcutMu = 1; // Kitap güncellendiğinde rafta kabul ediliyor
            strcpy(kitaplar[i].teslimTarihi, ""); // Teslim tarihi temizleniyor

            kitaplariKaydet(kitaplar, kitapSayisi);
            printf("Kitap güncellendi.\n");
            return;
        }
    }

    printf("Kitap bulunamadı.\n");
}

void kitapSil(Kitap kitaplar[], int *kitapSayisi) {
    if (*kitapSayisi == 0) {
        printf("Silinecek kitap bulunmamaktadır.\n");
        return;
    }
    printf("--- KİTAP SİLME ---\n");
    int id;
    printf("Silinecek kitap ID: ");
    scanf("%d", &id);

    for (int i = 0; i < *kitapSayisi; i++) {
        if (kitaplar[i].id == id) {
            for (int j = i; j < *kitapSayisi - 1; j++) {
                kitaplar[j] = kitaplar[j + 1];
            }
            (*kitapSayisi)--;
            kitaplariKaydet(kitaplar, *kitapSayisi);
            printf("Kitap silindi.\n");
            return;
        }
    }

    printf("Kitap bulunamadı.\n");
}

void gecikmeleriKontrolEt(Kullanici kullanicilar[], int kullaniciSayisi, Kitap kitaplar[], int kitapSayisi) {
    ekrani_temizle();
    if (kitapSayisi == 0) {
        printf("Gecikme kontrol edilecek kitap bulunmamaktadır.\n");
        return;
    }
    printf("--- GECİKMİŞ KİTAPLAR ---\n");
    for (int i = 0; i < kullaniciSayisi; i++) {
        for (int j = 0; j < kullanicilar[i].oduncSayisi; j++) {
            int kitapID = kullanicilar[i].oduncKitaplar[j];
            for (int k = 0; k < kitapSayisi; k++) {
                if (kitaplar[k].id == kitapID && kitaplar[k].mevcutMu == 0) {
                    printf("Kullanici: %s - Kitap: %s - Teslim Tarihi: %s\n",
                           kullanicilar[i].kullaniciAdi, kitaplar[k].ad, kitaplar[k].teslimTarihi);
                }
            }
        }
    }
    printf("Geçikmiş kitap kontrolü tamamlandı.\n");
}





void yoneticiMenusu(Kullanici *yonetici, Kullanici kullanicilar[], int *kullaniciSayisi, Kitap kitaplar[], int *kitapSayisi) {
    int secim;
    do {
        ekrani_temizle();
        printf("\n--- YONETİCİ MENÜSÜ ---\n");
        printf("1. Yeni kullanici ekle\n");
        printf("2. Kitap ekle\n");
        printf("3. Kitap güncelle\n");
        printf("4. Kitap sil\n");
        printf("5. Gecikmeleri kontrol et\n");
        printf("0. Çıkış\n");
        printf("Seçiminiz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                ekrani_temizle();
                yeniKullaniciEkle(kullanicilar, kullaniciSayisi);
                break;
            case 2:
                ekrani_temizle();
                kitapEkle(kitaplar, kitapSayisi);
                break;
            case 3:
                ekrani_temizle();
                kitapGuncelle(kitaplar, *kitapSayisi);
                break;
            case 4:
                ekrani_temizle();
                kitapSil(kitaplar, kitapSayisi);
                break;
            case 5:
                ekrani_temizle();
                gecikmeleriKontrolEt(kullanicilar, *kullaniciSayisi, kitaplar, *kitapSayisi);
                break;
            case 0:
                ekrani_temizle();
                printf("Çıkış yapiliyor...\n");
                break;
            default:
                ekrani_temizle();
                printf("Geçersiz seçim.\n");
        }

    } while (secim != 0);
}



void kullaniciMenusu(Kullanici *aktifKullanici, Kitap kitaplar[], int kitapSayisi) {
    int secim;
    do {
        ekrani_temizle();
        printf("\n--- KULLANICI MENÜSÜ (%s) ---\n", aktifKullanici->kullaniciAdi);
        printf("1. Kitap Ara\n");
        printf("2. Kitap Ödünç Al\n");
        printf("3. Kitap İade Et\n");
        printf("4. Ödünç Aldığım Kitaplar\n");
        printf("0. Çıkış\n");
        printf("Seciminiz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                ekrani_temizle();
                kitapAra(kitaplar, kitapSayisi);
                break;
            case 2:
                ekrani_temizle();
                kitapOduncAl(aktifKullanici, kitaplar, kitapSayisi);
                break;
            case 3:
                ekrani_temizle();
                kitapIadeEt(aktifKullanici, kitaplar, kitapSayisi);
                break;
            case 4:
                ekrani_temizle();
                kitaplarimiGoster(aktifKullanici, kitaplar, kitapSayisi);
                break;
            case 0:
                ekrani_temizle();
                printf("Çıkış yapılıyor...\n");
                break;
            default:
                ekrani_temizle();
                printf("Geçersiz Seçim.\n");
        }

    } while (secim != 0);
}


void kitapAra(Kitap kitaplar[], int kitapSayisi) {
    char arama[50];
    int bulundu = 0;
    ekrani_temizle();
    printf("--- KİTAP ARAMA ---\n");
    if (kitapSayisi == 0) {
        printf("Aranacak kitap bulunmamaktadır.\n");
        return;
    }
    printf("Aranacak Kitap Adı: ");
    getchar(); // \n temizliği
    fgets(arama, 50, stdin);
    arama[strcspn(arama, "\n")] = '\0';

    printf("\n--- Arama Sonuçarı ---\n");
    for (int i = 0; i < kitapSayisi; i++) {
        if (strstr(kitaplar[i].ad, arama) != NULL) {
             bulundu = 1;
             printf("Kitap ID: %d\n", kitaplar[i].id);
             printf("Kitap Adı: %s\n", kitaplar[i].ad);
             printf("Kitap Yazarı: %s\n", kitaplar[i].yazar);
             printf("Kitap Kategori: %s\n", kitaplar[i].kategori);
             printf("Mevcut Mu: %s\n", kitaplar[i].mevcutMu ? "Evet" : "Hayır");
                if (kitaplar[i].mevcutMu == 0) {
                    printf("Teslim Tarihi: %s\n", kitaplar[i].teslimTarihi);
                } else {
                    printf("Teslim Tarihi: Rafta\n");
                }
                printf("-------------------------\n");  
        }
            
           
        }
    if (!bulundu) {
        printf("Aradığınız kitap bulunamadı.\n");
        tusa_bas();
    }
    else {
        tusa_bas();
    }
}

void kitapOduncAl(Kullanici *kullanici, Kitap kitaplar[], int kitapSayisi) {
    if (kullanici->oduncSayisi >= MAKS_ODUNC) {
        printf("Maksimum odunc alabileceginiz kitap sayisina ulastiniz.\n");
        return;
    }

    int kitapID;
    printf("Odunc alinacak kitap ID: ");
    scanf("%d", &kitapID);

    for (int i = 0; i < kitapSayisi; i++) {
        if (kitaplar[i].id == kitapID && kitaplar[i].mevcutMu == 1) {
            kitaplar[i].mevcutMu = 0;

            // Teslim tarihi = bugünün tarihi + 15 gün
            time_t simdi = time(NULL);
            simdi += 15 * 24 * 60 * 60;
            struct tm *teslim = localtime(&simdi);
            strftime(kitaplar[i].teslimTarihi, 11, "%Y-%m-%d", teslim);

            for (int j = 0; j < MAKS_ODUNC; j++) {
                if (kullanici->oduncKitaplar[j] == -1) {
                    kullanici->oduncKitaplar[j] = kitapID;
                    kullanici->oduncSayisi++;
                    printf("Kitap odunc alindi. Teslim tarihi: %s\n", kitaplar[i].teslimTarihi);
                    return;
                }
            }
        }
    }

    printf("Kitap uygun degil veya bulunamadi.\n");
}



void kitapIadeEt(Kullanici *kullanici, Kitap kitaplar[], int kitapSayisi) {
    int kitapID;
    printf("Iade edilecek kitap ID: ");
    scanf("%d", &kitapID);

    for (int i = 0; i < kullanici->oduncSayisi; i++) {
        if (kullanici->oduncKitaplar[i] == kitapID) {
            for (int j = 0; j < kitapSayisi; j++) {
                if (kitaplar[j].id == kitapID) {
                    kitaplar[j].mevcutMu = 1;
                    strcpy(kitaplar[j].teslimTarihi, "");
                    break;
                }
            }

            kullanici->oduncKitaplar[i] = -1;
            kullanici->oduncSayisi--;
            printf("Kitap iade edildi.\n");
            return;
        }
    }

    printf("Bu kitap sizin tarafinizdan odunc alinmamis.\n");
}

void kitaplarimiGoster(Kullanici *kullanici, Kitap kitaplar[], int kitapSayisi) {
    printf("--- UZERINDEKI KITAPLAR ---\n");
    int bulundu = 0;
    for (int i = 0; i < MAKS_ODUNC; i++) {
        int kitapID = kullanici->oduncKitaplar[i];
        if (kitapID != -1) {
            for (int j = 0; j < kitapSayisi; j++) {
                if (kitaplar[j].id == kitapID) {
                    printf("ID: %d | Ad: %s | Teslim Tarihi: %s\n",
                           kitaplar[j].id, kitaplar[j].ad, kitaplar[j].teslimTarihi);
                    bulundu = 1;
                }
            }
        }
    }

    if (!bulundu) {
        printf("Uzerinizde kitap bulunmamaktadir.\n");
    }
}




#include "krgkargolistesi.h"
#include "ui_krgkargolistesi.h"

#include <Veri/VeriListesi/krgkargobilgileri.h>
#include <Veri/VeriListesi/krgalicibilgileri.h>
#include <Veri/VeriListesi/krggondericibilgileri.h>
#include <UI/VeriFormlari/krgyenikargoekle.h>
#include <Veri/krggenelveriyoneticisi.h>
#include <UI/VeriDuzenleme/krgaliciduzenleme.h>
#include <UI/VeriDuzenleme/krggondericiduzenleme.h>
#include <UI/VeriDuzenleme/krgkargoduzenleme.h>
#include<QStringList>
#include <QTableWidgetItem>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>


KRGKargoListesi::KRGKargoListesi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KRGKargoListesi)
{
    ui->setupUi(this);
    ara();
}

KRGKargoListesi::~KRGKargoListesi()
{
    delete ui;
}

void KRGKargoListesi::listeGuncelle()
{
    ui->tableWidgetKargoListesi->clear();
    ui->tableWidgetKargoListesi->setRowCount(listeKargo.length());
    ui->tableWidgetKargoListesi->setColumnCount(14);

    QStringList basliklar;
    basliklar <<tr("ID") << tr("Alıcı İd") << tr("Gönderici Id") << tr("Tarih") << tr("Ödeme Türü") << tr("En") << tr("Boy") << tr("Yükseklik") << tr("Desi") << tr("Gönderen Şube") << tr("Alıcı Şube") << tr("Tutar") <<tr("Gönderi Silme") << tr("Gönderi Düzenleme");
    ui->tableWidgetKargoListesi->setHorizontalHeaderLabels(basliklar);


    for (int i = 0; i < listeKargo.length(); i++) {


        QTableWidgetItem *hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeKargo[i]->getId()));
        ui->tableWidgetKargoListesi->setItem(i,0,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeKargo[i]->getKargoUcreti()));
        ui->tableWidgetKargoListesi->setItem(i,1,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeKargo[i]->getKargoUcreti()));
        ui->tableWidgetKargoListesi->setItem(i,2,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(listeKargo[i]->getKargoTarihi().toString("yyyy.MM.dd"));
        ui->tableWidgetKargoListesi->setItem(i,3,hucre);



        hucre = new QTableWidgetItem();
        switch (listeKargo[i]->getOdemeTuru()) {
        case OTGondericiOdemeli:
            hucre->setText("Gönderici Ödemeli");
            break;
        default:
            hucre->setText("Alıcı Ödemeli");
        }
        ui->tableWidgetKargoListesi->setItem(i, 4, hucre);



        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeKargo[i]->getKargoEn()));
        ui->tableWidgetKargoListesi->setItem(i,5,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeKargo[i]->getKargoBoy()));
        ui->tableWidgetKargoListesi->setItem(i,6,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeKargo[i]->getKargoYukseklik()));
        ui->tableWidgetKargoListesi->setItem(i,7,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeKargo[i]->getKargoDesi()));
        ui->tableWidgetKargoListesi->setItem(i,8,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(listeKargo[i]->getGonderenSube());
        ui->tableWidgetKargoListesi->setItem(i,9,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(listeKargo[i]->getAliciSube());
        ui->tableWidgetKargoListesi->setItem(i,10,hucre);


        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeKargo[i]->getKargoUcreti()));
        ui->tableWidgetKargoListesi->setItem(i,11,hucre);

        QPushButton *silmeButonu = new QPushButton();
        silmeButonu->setText(tr("Gönderi Sil"));
        ui->tableWidgetKargoListesi->setCellWidget(i,12,silmeButonu);

        QPushButton *duzenlemeButonu = new QPushButton();
        duzenlemeButonu->setText(tr("Gönderi Düzenleme"));
        ui->tableWidgetKargoListesi->setCellWidget(i,13,duzenlemeButonu);


        auto veri_i = listeKargo[i];

        connect(silmeButonu, &QPushButton::clicked, [veri_i,this]() {
            auto cevap = QMessageBox::question(nullptr, tr("Silme Onayı"), tr("%1 isimli gönderiyi silmek istediğinize emin misiniz?").arg(veri_i->getId()));
            if (cevap == QMessageBox::Yes) {
                KRGGenelVeriYoneticisi::db().getKargoBilgileri().sil(veri_i->getId());
                QMessageBox::information(nullptr, tr("Kayıt Silindi"), tr("Kayıt silme işlemi tamamlandı!"));
                this->ara();
            }
        });

        connect(duzenlemeButonu, &QPushButton::clicked, [veri_i,this]() {
            KRGKargoDuzenleme form;
            form.setVeri(veri_i);
            form.setWindowTitle(tr("%1 Id'li Gönderiyi Düzenle").arg(veri_i->getId()));

            if (form.exec() == QDialog::Accepted) {
                form.getVeri();
                this->listeGuncelle();
            }
        });



    }

    ui->tableWidgetAliciListesi->clear();
    ui->tableWidgetAliciListesi->setRowCount(listeAlici.length());
    ui->tableWidgetAliciListesi->setColumnCount(5);


    QStringList basliklarAlici;
    basliklarAlici << tr("id") << tr("Alıcı Adı") << tr("Telefon Numarası") << tr("Mail") << tr("Adresi");
    ui->tableWidgetAliciListesi->setHorizontalHeaderLabels(basliklarAlici);

    for (int i = 0; i < listeAlici.length(); i++) {

        QTableWidgetItem *hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeAlici[i]->getId()));
        ui->tableWidgetAliciListesi->setItem(i,0,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(listeAlici[i]->getAliciAdi());
        ui->tableWidgetAliciListesi->setItem(i,1,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(listeAlici[i]->getAliciTelNo());
        ui->tableWidgetAliciListesi->setItem(i,2,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeAlici[i]->getAliciEmail()));
        ui->tableWidgetAliciListesi->setItem(i,3,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeAlici[i]->getAliciAdresi()));
        ui->tableWidgetAliciListesi->setItem(i,4,hucre);

    }

    ui->tableWidgetGondericiListesi->clear();
    ui->tableWidgetGondericiListesi->setRowCount(listeGonderici.length());
    ui->tableWidgetGondericiListesi->setColumnCount(5);

    QStringList basliklarGonderici;
    basliklarGonderici << tr("id") << tr("Gonderici Adı") << tr("Telefon Numarası") << tr("Mail") << tr("Adresi");
    ui->tableWidgetGondericiListesi->setHorizontalHeaderLabels(basliklarGonderici);

    for (int i = 0; i < listeGonderici.length(); i++) {

        QTableWidgetItem *hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeGonderici[i]->getId()));
        ui->tableWidgetGondericiListesi->setItem(i,0,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(listeGonderici[i]->getGonderenAdi());
        ui->tableWidgetGondericiListesi->setItem(i,1,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(listeGonderici[i]->getGonderenTelNo());
        ui->tableWidgetGondericiListesi->setItem(i,2,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeGonderici[i]->getGonderenEmail()));
        ui->tableWidgetGondericiListesi->setItem(i,3,hucre);

        hucre = new QTableWidgetItem();
        hucre->setText(tr("%1").arg(listeGonderici[i]->getGonderenAdresi()));
        ui->tableWidgetGondericiListesi->setItem(i,4,hucre);

    }


}



/*
 * Lineer Arama Algoritması yazıldı.
 * Vakit az diye diğer algoritmalardan vazgeçildi.
 *
 */



void KRGKargoListesi::ara()
{
    // quint64 aramaAliciId;
    auto ekran = this->ui;
    listeKargo = KRGGenelVeriYoneticisi::db().getKargoBilgileri().tumunuBul(
        [ekran](KRGKargoBilgileriYoneticisi::Ptr veri)->bool {
            if(ekran->lineEditKargoIdGiriniz->text()==""){
                return true;
            }
            auto iter = QString::number(veri->getId());
            if (ekran->lineEditKargoIdGiriniz->text()!=""){
                if(iter != ekran->lineEditKargoIdGiriniz->text()){
                    // aramaAliciId = veri->getAliciId();
                    return false;
                }
            }
            return true;
        });
    listeAlici = KRGGenelVeriYoneticisi::db().getAliciBilgileri().tumunuBul(
        [](KRGAliciBilgileriYoneticisi::Ptr veri)->bool{
            return true;
            }
        );

    listeGonderici = KRGGenelVeriYoneticisi::db().getGondericiBilgileri().tumunuBul(
        [](KRGGondericiBilgileriYoneticisi::Ptr veri)->bool {
            return true;
            }
        );
    listeGuncelle();
}

void KRGKargoListesi::on_pushButtonAra_clicked()
{
    ara();
}


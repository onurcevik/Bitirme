# projenin ana parçaları - akış şeması
1. insansız hava aracından görüntü alımı (görüntü ham ya da kodlanmış olabilir, kodlanmış görüntü mpeg4, h265, divx vb. bir kodlama tekniği kullanabilir)
2. insansız hava aracında görüntü şifreleme (görüntü ham ya da kodlanmış olabilir, şifreleme modülü ham asci blokunu şifreleyip deşifreleme yapacağı için bu kısım ayrı bir modül olarak düşünülmüştür)
3. görüntü iletimi (UDP protokolü ve 2.4GHz radyo frekansları kullanılması düşünülmektedir)
4. istasyon makinesinde deşifreleme (şifrelemenin tersi)
5. istasyon makinesine gelen görüntü üzerinde nesne takibi (nesne takibi yaparken eşzamanlı olarak insansız hava aracına nesneyi takip etmek üzere yeni koordinatlar gönderimi olacaktır.)

## nesne takibi
1. başlatımlama: takip edilecek nesne seçilir (seçim dikdörtgen ya da kapalı bir poligon ile olabilir, ilk aşamada dikdörtgen olacak, seçilen kapalı bölge nesneyi temsil edecek)
  * ileriki aşamalarda nesne manuel olarak değil de otomatik nesne tanıma yöntemi ile seçilmesi düşünülmektedir, ancak bu projenin bir parçası değildir.
2. insansız hava aracına kameranın koordinatının değişimi de gönderilmelidir, bu kısım istasyondaki nesne takip yöntemi ile belirlenecektir.
3. nesne takip ederken nesnenin 3 invaryantı dikkate alınmalıdır (öteleme, ölçekleme, dönme)
  * öteleme işlemi doğal olarak nesne takibini gerektiriyor,
  * dönme işlemi takip edilecek nesnelerde olabilecek bir durum, yapılması gerekir, algoritmalara bakmak gerekir
  * ölçekleme: nesne kameranın yakınlaşıp uzaklaşmasına göre büyüyüp küçülebilir, nesne takibi buna dayanıklı olsa iyi olur
4. istasyonda takip yapılırken hesaplanan bir sonraki öteleme, dönme ve ölçekleme değerleri insansız hava aracına sürekli gönderilmelidir.
5. İHA: insansız hava aracı, ST:istasyon
  * sistem İHA -> ST -> İHA -> ST -> .... döngüsü ile çalışmalıdır
  * İHA: görüntü al, kodla, şifrele, gönder ---> yeni parametreleri al, ayarla, görüntü al, kodla, şifrele, gönder ---> ... döngüsü ile çalışır
  : ST: görüntü al, deşifrele, nesne takibi ile yeni dönme-öteleme-ölçekleme parametrelerini belirle, İHA'ya gönder .... döngüsü ile çalışır.
  
## Tübitak Projesinde Dikkat Edilecekler
1. kodlama (?duruma göre), şifreleme, deşifreleme olacak
2. görüntü iletimi ve alma olacak
3. nesne takibi için
  * başlangıçta takip edilecek nesne seçilecek (ileride ilgili nesne, makine öğrenmesi ile otomatik bulunabilir diye vurgulanabilir)
  * nesne hareket ettikçe dönme-öteleme-ölçekleme parametreleri güncellenecek,
  * nesne kaybolduğunda (nesne kamera açısından çıkabilir, köprü ya da ağaçlık bir alana girebilir) en sonki görüntüsü ve gidebileceği yer, yön gibi bilgiler kullanılarak yeniden nesneye odaklanılması gerekir, bunun için algoritmalar var)
4. B Planı: İHA vb. uçan bir araç çeşitli nedenlerle temin edilememsi durumunda bu sistemin ikizi kara araçlarında denenecektir.
5. +++ rapor genel olarak güzel oldu, bazı şeyler önereceğim;
  * öğrenme ve tanıma kısmını iş parçalarından çıkarın,
  * yukarıda yazdıklarımı akış şeması olarak yeniden düzenleyebilirsiniz,
  * literatür araştırmalarından biraz bahsedebilirsiniz,
  * B planından bahsedebilirsiniz,
  * bunların dışında fazla uzatmadan toparlayın, gönderin, gitsin, 
  * kolay gelsin...

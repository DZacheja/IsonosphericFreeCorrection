# IsonosphericFreeCorrection
Program do obliczania poprawki jonosferycznej
<b> praca magisterska - Damian Zacheja </b>

## Cel utworzenia oprogramowania
Wyznaczenie pozycji odbiornika można wykonać na wiele sposobów. Każdy z nich jest obarczony błędami stałymi oraz zmiennymi. Niwelowanie ich wpływów wykonuje się na kilka sposobów. Program niweluje błędy powstałe w jonosferze. Warstwa ta rozciąga się od 50 do 500 km nad powierzchnią Ziemi i z powodu występowania w niej zjonizowanych cząsteczek, zakłóca sygnał.<br> 
Kombinacja liniowa iono-free wykorzystuje pomiary dwuczęstotliwościowe, których kombinacja pozwala na wyeliminowanie błędu spowodowanego tą warstwą atmosfery.<br>
Satelity modulują kody na różnych częstotliwościach. Program umożliwia sprawdzenie różnych wariantów kobminacji i określenie, która daje najelepsze rezultaty.<br<br>

## Opis Programu
### Okno główne programu:
![GłówneOkno](https://github.com/DZacheja/IsonosphericFreeCorrection/blob/master/Images/Description/MainWindow.png "Okno Główne programu")
<br>
W oknie mamy możliwość skazania pliku zawirającego obserwacje z urządzenia oraz plik brodecast. <br>
W kolejnym etapie wybieramy parametry obliczeń i czekamy na wyniki.
Dokładna dokumentacja znajduje się w pracy magisterskiej.

### Okno z wynikami 
![Wyniki](https://github.com/DZacheja/IsonosphericFreeCorrection/blob/master/Images/Description/MainWindowAfterResults.png "Wyniki")
<br>Po wykoananych obliczeniach mamy możliwość podglądu ostatecznie wyliczonych współrzędnych dla każdej epoki.

### Wyniki pośrednie obliczeń
![WynikiPośrednie](https://github.com/DZacheja/IsonosphericFreeCorrection/blob/master/Images/Description/IndirectbueResults.png "Wyniki pośrednie")
Dla każdej obliczonej epoki jest możliwośc sprawdzenia wszystkich pośrednich obliczeń w celu dokładnej analizy.

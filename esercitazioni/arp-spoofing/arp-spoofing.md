Per eseguire questo attacco dobbiamo, per ogni partecipante fare il ping degli altri cosi da avere 
una situazione stabile di partenze. 
Quindi per ogni host facciamo `ping <host-ip>`
Controlliamo che tutto è stato effettuato correttamente con `arp -a`     

Vogliamo quindi avvelenare la tabla arp della vittima cosi da far si che lei invii a noi tutti i messaggi che dovrebbe inviare all'host 
generico. 

Per fare questo dobbiamo craftare un pacchetto arp.
Lo facciamo attraverso `scapy`

craftiamo il pacchetto nel seguente modo: 

1. creiamo un pacchetto arp `arpPackage = ARP()` 
2. lo mandiamo alla vittima con `arpPackage.hwdst:"<MAC-Vittima>"` e `arpPackage.pdst:"<ip-vittima>"` 
3. indichiamo che noi siamo l'host con `arpPackage.psrc:"<ip-host>"` 
4. indichiamo che lo deve girare non al MAC dell'host, bensì al nostro `arpPackage.hwsrc:"<MAC-eve>"` 
5. inviamo il pacchetto :) `send(arpPackage)`

ora possiamo controllare se la tabella arp della vittima e controllare se abbiamo effettuato correttamente lo spoofing. 
In docker non so perche ma non funziona

- apriamo le tra macchine 
- lanciamo `apt update` e poi `apt install ping` e poi selezionare l'opzione accurata
- apriamo metasploitable e lanciamo `telnet <Ip-di-metasploitable> 23` 
- lanciamo lo stesso comando su host
- lanciamo ora tshark dalla kali cattiva 
- comunichiamo tra host e metasploitable e osserviamo i pacchetti dalla kali cattiva

dobbiamo fare tutto nella kali cattiva verso l'host buono. logghiamo, attendiamo che telent si svegli e in un istanza della kali cattiva 
ci connettiamo a metasploitable tramite telnet. Segue:

- apriamo termshark sulla kali cattiva 
- lanciamo `telnet localhost 23` dall'altra istanza della kali cattiva 
- osserviamo come i pacchetti Telnet mostrino informazioni sulla login

per farlo con ssh dobbiamo fare piu o meno la stessa cosa solo che in questo caso dobbiamo fare 

- `ssh -oHostKeyAlgorithms=+ssh-dss msfadmin@<Ip-di-metasploitable>`

notimao come in questo caso i pacchetti SSH hanno il field "encrypted"

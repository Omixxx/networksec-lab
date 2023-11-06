- creaiamo un file di testo lanciando `echo ciao come stai? > plain.txt`
- cifriamolo lanciando `openssl aes-128-cbc  -e -in  plain.txt -out aes-cbc-cipher.bin`


--- 
per le immagini

- cifriamo l'immagine in chiaro con `openssl aes-128-cbc -e -in pic_original.bmp -out aes-cbc-pic.bmp` per cbc 
- cifriamo l'immagine in chiaro con `openssl aes-128-ecb -e -in pic_original.bmp -out aes-ecb-pic.bmp` per ecb
- copiamo i primi 108 bytes (credo) dall'immagine originale all'immagine con `dd bs=108 count=1 conv=notrunc if=pic_original.bmp of=aes-ecb-pic.bmp`
- copiamo i primi 108 bytes (credo) dall'immagine originale all'immagine con `dd bs=108 count=1 conv=notrunc if=pic_original.bmp of=aes-cbc-pic.bmp`
- notiamo come la prima nasconda ancora informazioni mentre la seconda non piu 

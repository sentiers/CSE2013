test5    start   1000                                            
first    j       begin                                           
five     word    5                                               
xxx      resw    1                                               
begin    lda     zero                                            
         ldx     zero                                            
loop     jsub    getc                                            
         mul     hex                                             
         sta     tmp                                             
         jsub    getc                                            
         add     tmp                                             
         stch    exaddr,x                                        
         tix     index                                           
         j       loop                                            
getc     stx     temp                                            
         ldx     incnt                                           
         lda     incnt                                           
         add     one                                             
         sta     incnt                                           
         ldch    indev,x                                         
         comp    eof                                             
         jeq     exaddr                                          
         comp    charzero                                        
         jlt     getc                                            
         sub     charzero                                        
         comp    ten                                             
         jlt     return                                          
         sub     seven                                           
return   ldx     temp                                            
         rsub                                                    
zero     word    0                                               
one      word    1                                               
seven    word    7                                               
ten      word    10                                              
hex      word    16                                              
charzero word    48                                              
eof      word    63                                              
tmp      resw    1                                               
temp     resw    1                                               
index    word    0                                               
incnt    word    0                                               
indev    byte    c'0010030C1006'                                 
         byte    c'000005FFFFFF?'                                
exaddr   resb    12                                              
         end     first                                           

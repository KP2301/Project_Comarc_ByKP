            lw          0           5           pos1       
            lw          0           1           input     
            lw          0           2           subAdr   
            jalr         2           4                       
            lw          0           5           pos1      
            lw          0           1           input      
            lw          0           2           subAdr   
            jalr         2           4                           
            halt
sub4n     sw         7           4           stack      
            add        7           5           7           
            sw         7           1           stack      
            add        7           5           7           
            add        1           1           1           
            add        1           1           3           
            lw          0           2           neg1       
            add        7           2           7           
            lw          7           1           stack      
            add        7           2           7           
            lw          7           4           stack      
            jalr         4           2                       
pos1       .fill         1
neg1       .fill         -1
subAdr   .fill         sub4n                             
input      .fill         10                                  
stack .fill    0

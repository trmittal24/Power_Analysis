start = 0;
counter = 1;
old = 0;
stop = 0;
times = zeros(2,56);
for i = 1:1919 
    if(start == 0)
        if(Trace_2(i,2) > -2)
            if(Trace_3(i,2)>-2)
                start = 1;          %start the operation when both traces cross logical "1" for the first time
            end
        end
    end
    
        
    if(start == 1 & stop==0)
    
    if(abs(old - Trace_3(i,2)) > 2)
        counter =  counter + 1;          %when it toggles,switch to next bit
        
        if(counter > 56)
            stop = 1;
            continue;
        end
    end
    times(mod(counter,2) + 1,counter) = times(mod(counter,2) + 1,counter) + 1; %when in the current bit, increment this value as long as it is here
    old = Trace_3(i,2) ;        %store this to compare in next loop

    
    end
end

times
   
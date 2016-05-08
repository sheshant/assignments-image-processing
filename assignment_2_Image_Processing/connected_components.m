function images = connected_components(threshold_image)
    count = 0;
    row = size(threshold_image,1);
    col = size(threshold_image,2);
    i = 1;
    while(i < row)
        % first part get the index of all zeros
        col_idx = find(threshold_image(i,:) == 255,1);
        row_idx = i;
        % we got index now we find all its 8 neighbours  
        if(isempty(col_idx) == 0)
            count = count+1;
            threshold_image(row_idx,col_idx) = count;
            neighbour = [row_idx col_idx];
            while(isempty(neighbour) == 0)
                test = [];
                for j = 1:size(neighbour,1)
                    row_idx = neighbour(j,1);
                    col_idx = neighbour(j,2);

                    for k = -1:1
                        for l = -1:1
                            if(row_idx-k > 0 && row_idx-k <= row && col_idx-l > 0 && col_idx-l <= col)
                                if((threshold_image(row_idx-k,col_idx-l)) == 255)
                                    threshold_image(row_idx-k,col_idx-l) = count;
                                    test = [test;[row_idx-k col_idx-l]];
                                end
                            end
                        end
                    end 
                end
                neighbour = test;
            end
        else
            i = i+1;
        end
    end
    % now all the connected components separately
    for i = 1:count
        dummy = threshold_image;
        dummy(dummy ~= i) = 0;
        dummy(dummy ~= 0) = 1;
        images(:,:,i) = dummy;
    end
end
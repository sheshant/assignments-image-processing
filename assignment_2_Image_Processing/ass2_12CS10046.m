%% keep on changing test image file name here
image = imread('J1.png');
%% parameters of image
channel = size(image,3);
row = size(image,1);
col = size(image,2);
%% rest RGB components 
red = image(:,:,1);
green = image(:,:,2);
blue = image(:,:,3);

%% grayscale
grayscale_image = rgb2gray(image);

%% thresholding 
threshold_image = grayscale_image;
threshold_image(threshold_image >= 240) = 0;
threshold_image(threshold_image ~= 0) = 255;

%% and the rest of the part 

er = threshold_image;
count = 1;
answer = [];
seclose = strel('disk',5,0);% this is to perform closing operation 
while(sum(er(:)) ~= 0)
    % until the image is complete black ie 
    % no component left
    parts = connected_components(er);
    % we got the connected components separately
    se = strel('disk',count,0);
    er2 = imerode(threshold_image,se);
    er2 = imclose(er2,seclose);
    % without close operation there will be unwanted connected components 
    er = er2;
    % and we got the image after erosion with new SE
    % now we need to check whether this particular connected component
    % vanishes or not
    image_count = size(parts,3);
    for i = 1:image_count
        test = parts(:,:,i).*er2;
        if(sum(test(:)) == 0)%then we should check this as it vanishes 
            a = more_details(parts(:,:,i),count-1,image);
            answer = [answer;a];% we got details about it now we need to update answer
        end
    end
    count = count+1;
end
%% now display image
s = size(answer,1);
fprintf('\t\tTotal number of candies = %d\n\n',s);
for i = 1:s
    a = answer(i,:);
    fprintf('For candy %d \n',i);
    fprintf('\tColour (R,G,B) = (%d,%d,%d)\n',a(2),a(3),a(4));
    fprintf('\tRadius = %d \n',a(1));
    if(a(5) == 0)
        fprintf('\tIt is not a perfect circle \n');
    else
        fprintf('\tIt is a perfect circle \n');
    end
end

function answer = more_details(threshold_image,se_count,colour_image)
    grayscale_image = rgb2gray(colour_image);
    bool = 1;% circle
    se1 = strel('disk',se_count+1,0);
    se2 = strel('disk',se_count+2,0);
    d1 = imdilate(threshold_image*255,se1);
    d2 = imdilate(threshold_image*255,se2);
    d1 = imcomplement(d1);
    final_image = (d1/255).*(d2/255);
    %% get the pixel values of the outer perimeter 
    [fx,fy] = find(final_image);
    len1 = length(fx);
    pixels = zeros(len1,1);
    for i = 1:len1
        pixels(i) = grayscale_image(fx(i),fy(i));
    end
    %% get the pixel value of the last unvanished connected component
    [x,y] = find(threshold_image);
    len = length(x);
    red = zeros(len,1);
    green = zeros(len,1);
    blue = zeros(len,1);
    gray = zeros(len,1);
    
    for i = 1:len
        red(i) = colour_image(x(i),y(i),1);
        green(i) = colour_image(x(i),y(i),2);
        blue(i) = colour_image(x(i),y(i),3);
        gray(i) = grayscale_image(x(i),y(i));
    end
    % now the colour
    red_val = mean(red);
    green_val = mean(green);
    blue_val = mean(blue);
    pixel_value = mean(gray);
    %% we need to check whether it is circle or not 
    %   if the value of pixel of outer perimeter is similar to inner then
    %   not circle
    if(any(pixels == pixel_value))
        bool = 0;
    end
    answer = [se_count red_val green_val blue_val bool];
end
function ChangeContrast()

% Load an image
originalImage = imread('lena_embed.bmp');
imwrite(originalImage, 'output/38.66lena_embed.bmp')
calculateContrast('lena_embed.bmp');
%% 改变对比度，J = imadjust(I,[low_in; high_in],[low_out; high_out])表示将low_in至high_in之间的值映射到low_out至high_out之间的值

% Change the image's contrast 0.15-0.85 to 0-1.0
contrastAdjustedImageA = imadjust(originalImage, [0.15 0.85], [0,1.0]);
% Save the adjusted image to a file
imwrite(contrastAdjustedImageA, 'output/55.69contrastAdjustedImage0.15-0.85.bmp')
calculateContrast('output/55.69contrastAdjustedImage0.15-0.85.bmp');

% Change the image's contrast 0.30-0.70 to 0-1.0
contrastAdjustedImageB = imadjust(originalImage, [0.30 0.70], [0,1.0]);
% Save the adjusted image to a file
imwrite(contrastAdjustedImageB, 'output/73.94contrastAdjustedImage0.30-0.70.bmp')
calculateContrast('output/73.94contrastAdjustedImage0.30-0.70.bmp');

% Change the image's contrast 0.45-0.55 to 0-1.0
contrastAdjustedImageC = imadjust(originalImage, [0.45 0.55], [0,1.0]);
% Save the adjusted image to a file
imwrite(contrastAdjustedImageC, 'output/91.77contrastAdjustedImage0.45-0.55.bmp')
calculateContrast('output/91.77contrastAdjustedImage0.45-0.55.bmp');

% Change the image's contrast 0-1.0 to 0.20-0.80
contrastAdjustedImageD = imadjust(originalImage, [0,1.0], [0.20 0.80]);
% Save the adjusted image to a file
imwrite(contrastAdjustedImageD, 'output/22.94contrastAdjustedImage0.20-0.80.bmp')
calculateContrast('output/22.94contrastAdjustedImage0.20-0.80.bmp');

% Change the image's contrast 0-1.0 to 0.40-0.60
contrastAdjustedImageE = imadjust(originalImage, [0,1.0], [0.40 0.60]);
% Save the adjusted image to a file
imwrite(contrastAdjustedImageE, 'output/7.56contrastAdjustedImage0.40-0.60.bmp')
calculateContrast('output/7.56contrastAdjustedImage0.40-0.60.bmp');


% Display the original and adjusted images
subplot(2,3,1), imshow(originalImage), title('Original Image')
subplot(2,3,2), imshow(contrastAdjustedImageA), title({'Contrast Adjusted Image';'0.05-0.95'})
subplot(2,3,3), imshow(contrastAdjustedImageB), title({'Contrast Adjusted Image';'0.10-0.90'})
subplot(2,3,4), imshow(contrastAdjustedImageC), title({'Contrast Adjusted Image';'0.15-0.85'})
subplot(2,3,5), imshow(contrastAdjustedImageD), title({'Contrast Adjusted Image';'0.20-0.80'})
subplot(2,3,6), imshow(contrastAdjustedImageE), title({'Contrast Adjusted Image';'0.40-0.60'})

end

function contrast = calculateContrast(filename)
  % 读取图像
  I = imread(filename);
  % 计算图像的平均值
  mean_value = mean(I(:));
  % 计算图像的标准差
  std_value = std(double(I(:)));
  % 计算图像的对比度
  contrast = std_value / mean_value;
  % 输出图像的对比度
  disp(contrast);
end




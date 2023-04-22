% Load an image
originalImage = imread('lena_embed.bmp');

%% Jpeg压缩，Quality字段表示压缩级别

% Set the compression level to 95
imwrite(originalImage, 'output/compressionImage95.jpg', 'quality', 95)

% Set the compression level to 90
imwrite(originalImage, 'output/compressionImage90.jpg', 'quality', 90)

% Set the compression level to 85
imwrite(originalImage, 'output/compressionImage85.jpg', 'quality', 85)

% Set the compression level to 80
imwrite(originalImage, 'output/compressionImage80.jpg', 'quality', 80)

% Set the compression level to 75
imwrite(originalImage, 'output/compressionImage75.jpg', 'quality', 75)

% Display the original image and compression image
compressionImage95 = imread('output/compressionImage95.jpg');
compressionImage90 = imread('output/compressionImage90.jpg');
compressionImage85 = imread('output/compressionImage85.jpg');
compressionImage80 = imread('output/compressionImage80.jpg');
compressionImage75 = imread('output/compressionImage75.jpg');
subplot(2,3,1), imshow(originalImage), title('Original Image')
subplot(2,3,2), imshow(compressionImage95), title('Compression Image 95')
subplot(2,3,3), imshow(compressionImage90), title('Compression Image 90')
subplot(2,3,4), imshow(compressionImage85), title('Compression Image 85')
subplot(2,3,5), imshow(compressionImage80), title('Compression Image 80')
subplot(2,3,6), imshow(compressionImage75), title('Compression Image 75')

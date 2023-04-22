% Load an image
originalImage = imread('lena_embed.bmp');

%% 添加高斯噪声，第三个参数表示噪声强度

% Add 1% Gaussian noise to the image
noisyImageA = imnoise(originalImage, 'gaussian', 0.01);
% Save the noisy image to a file
imwrite(noisyImageA, 'output/noisyImage0.01.bmp')

% Add 2% Gaussian noise to the image
noisyImageB = imnoise(originalImage, 'gaussian', 0.02);
% Save the noisy image to a file
imwrite(noisyImageB, 'output/noisyImage0.02.bmp')

% Add 3% Gaussian noise to the image
noisyImageC = imnoise(originalImage, 'gaussian', 0.03);
% Save the noisy image to a file
imwrite(noisyImageC, 'output/noisyImage0.03.bmp')

% Add 5% Gaussian noise to the image
noisyImageD = imnoise(originalImage, 'gaussian', 0.05);
% Save the noisy image to a file
imwrite(noisyImageD, 'output/noisyImage0.05.bmp')

% Add 7% Gaussian noise to the image
noisyImageE = imnoise(originalImage, 'gaussian', 0.07);
% Save the noisy image to a file
imwrite(noisyImageE, 'output/noisyImage0.07.bmp')

% Add 10% Gaussian noise to the image
noisyImageF = imnoise(originalImage, 'gaussian', 0.10);
% Save the noisy image to a file
imwrite(noisyImageF, 'output/noisyImage0.10.bmp')


% Display the original and noisy images
subplot(2,4,1), imshow(originalImage), title('Original Image')
subplot(2,4,2), imshow(noisyImageA), title('Noisy Image 0.01')
subplot(2,4,3), imshow(noisyImageB), title('Noisy Image 0.02')
subplot(2,4,4), imshow(noisyImageC), title('Noisy Image 0.03')
subplot(2,4,5), imshow(noisyImageD), title('Noisy Image 0.05')
subplot(2,4,6), imshow(noisyImageE), title('Noisy Image 0.07')
subplot(2,4,7), imshow(noisyImageF), title('Noisy Image 0.10')

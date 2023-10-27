import torch
import torch.nn as nn

class encoder(nn.Module):
    def __init__(self, in_channels):
        super(encoder, self).__init__()
        slope = 0

        self.conv1 = nn.Sequential(
            nn.Conv2d(in_channels=in_channels, out_channels=64, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.Conv2d(in_channels=64, out_channels=64, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),
        )

        self.maxpool1 = nn.MaxPool2d(kernel_size=2)

        self.conv2 = nn.Sequential(
            nn.Conv2d(in_channels=64, out_channels=128, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.Conv2d(in_channels=128, out_channels=128, kernel_size=3, padding=1),
            nn.LeakyReLU(slope)
        )

        self.maxpool2 = nn.MaxPool2d(kernel_size=2)

        self.conv3 = nn.Sequential(
            nn.Conv2d(in_channels=128, out_channels=256, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),
    
            nn.Conv2d(in_channels=256, out_channels=256, kernel_size=3, padding=1),
            nn.LeakyReLU(slope)
        )

        self.maxpool3 = nn.MaxPool2d(kernel_size=2)

        self.conv4 = nn.Sequential(
            nn.Conv2d(in_channels=256, out_channels=512, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),
    
            nn.Conv2d(in_channels=512, out_channels=512, kernel_size=3, padding=1),
            nn.LeakyReLU(slope)
        )

        self.maxpool4 = nn.MaxPool2d(kernel_size=2)

        self.finalconv = nn.Sequential(
            nn.Conv2d(in_channels=512, out_channels=1024, kernel_size=3, padding=1),
            nn.LeakyReLU(slope)
        )

    def forward(self, x):

        conv1 = self.conv1(x)
        maxpool1 = self.maxpool1(conv1)
        
        conv2 = self.conv2(maxpool1)
        maxpool2 = self.maxpool2(conv2)

        conv3 = self.conv3(maxpool2)
        maxpool3 = self.maxpool3(conv3)

        conv4 = self.conv4(maxpool3)
        maxpool4 = self.maxpool4(conv4)

        finalconv = self.finalconv(maxpool4)

        return conv1, conv2, conv3, conv4, finalconv

class decoder(nn.Module):
    def __init__(self, out_channels):
        super(decoder, self).__init__()
        slope = 0        
        self.mode = "nearest"

        self.firstconv = nn.Sequential(
            nn.Conv2d(in_channels=1024, out_channels=1024, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.ConvTranspose2d(in_channels=1024, out_channels=512, kernel_size=2, stride=2)
        )

        self.conv1 = nn.Sequential(
            nn.Conv2d(in_channels=1024, out_channels=512, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.Conv2d(in_channels=512, out_channels=512, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.ConvTranspose2d(in_channels=512, out_channels=256, kernel_size=2, stride=2)
        )

        self.conv2 = nn.Sequential(
            nn.Conv2d(in_channels=512, out_channels=256, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.Conv2d(in_channels=256, out_channels=256, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.ConvTranspose2d(in_channels=256, out_channels=128, kernel_size=2, stride=2)
        )

        self.conv3 = nn.Sequential(
            nn.Conv2d(in_channels=256, out_channels=128, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.Conv2d(in_channels=128, out_channels=128, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.ConvTranspose2d(in_channels=128, out_channels=64, kernel_size=2, stride=2)
        )

        self.finalconv = nn.Sequential(
            nn.Conv2d(in_channels=128, out_channels=64, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.Conv2d(in_channels=64, out_channels=64, kernel_size=3, padding=1),
            nn.LeakyReLU(slope),

            nn.Conv2d(in_channels=64, out_channels=out_channels, kernel_size=1),
            nn.Sigmoid()
        )

    def forward(self, en1, en2, en3, en4, f):

        de1 = self.firstconv(f)
        de1 = torch.cat([de1, en4], dim=1)

        de2 = self.conv1(de1)
        de2 = torch.cat([de2, en3], dim=1)

        de3 = self.conv2(de2)
        de3 = torch.cat([de3, en2], dim=1)

        de4 = self.conv3(de3)
        de4 = torch.cat([de4, en1], dim=1)

        return self.finalconv(de4)


class unet(nn.Module):
    def __init__(self, in_channels, out_channels):
        super(unet, self).__init__()
        self.encoder = encoder(in_channels)
        self.decoder = decoder(out_channels)

    def forward(self, x):

        c1, c2, c3, c4, f = self.encoder(x)
        out = self.decoder(c1, c2, c3, c4, f)
        return out


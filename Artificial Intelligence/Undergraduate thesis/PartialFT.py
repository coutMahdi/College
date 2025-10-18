import torch
import torch.nn as nn
from torchvision import datasets, transforms, models
from torch.utils.data import DataLoader, random_split, Subset
import os
import time

import numpy as np
import random

import matplotlib.pyplot as plt # type: ignore

def set_seed(seed=42): # هرعدد ثابتی فرقی ندارد RNG input
    # Python random
    random.seed(seed)
    # Numpy
    np.random.seed(seed)
    # PyTorch CPU
    torch.manual_seed(seed)
    # PyTorch GPU
    torch.cuda.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)  # اگر چند GPU دارید

    # تنظیم deterministic و غیرفعال کردن benchmark برای CUDA
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False

# ست کردن seed قبل از هر کار تصادفی
set_seed(42)

# --- Config ---
data_dir = r"E:\Datasets\Diabetic Retinopathy 224x224 Gaussian Filtered\gaussian_filtered_images"
model_path = r"E:\Datasets\Diabetic Retinopathy 224x224 Gaussian Filtered\Models\PT_100.pt"

if not os.path.exists(data_dir):
    print(f"Error: Dataset directory not found at '{data_dir}'")
    exit()

image_size = 224    #  B0=224,  B1=240, B2=260, B3=300
batch_size = 16
learning_rate = 1e-3
epochs = 20
num_workers = 0 # to increase speed. 0 subprocess(=worker) in parallel alowed --> burdens cpu # but bcz of windows poor management of multiprocessing ...
pin_memory = True
patience = 4
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

if __name__ == '__main__':
    print(f"Using device: {device}")

    # --- Data Loading ---
    transform = transforms.Compose([
        transforms.Resize((image_size, image_size)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406],
                             std=[0.229, 0.224, 0.225])
    ])

    full_dataset = datasets.ImageFolder(data_dir, transform=transform)
    num_classes = len(full_dataset.classes)
    print(f"Found {len(full_dataset)} images belonging to {num_classes} classes.")

    # N = len(full_dataset)
    # N_test = int(0.2 * N)
    # N_val = int(0.1 * (N - N_test))
    # N_train = N - N_test - N_val
    # train_ds, val_ds, test_ds = random_split(full_dataset, [N_train, N_val, N_test])

    # test_indices = test_ds.indices  # اندیس‌های نمونه‌های تست،no random_split in test bcz it'll be different than this needed to be saved
    # torch.save(test_indices, r"E:\Datasets\Diabetic Retinopathy 224x224 Gaussian Filtered\test_indices.pth") # حفط اسپلیت با ذخیره و قابل پاس دادن کردن سمپل ها به فایل تست

    #     # ذخیره اندیس‌ها
    # torch.save({
    #     'train_indices': train_ds.indices,
    #     'val_indices': val_ds.indices,
    #     'test_indices': test_ds.indices
    # }, r"E:\Datasets\Diabetic Retinopathy 224x224 Gaussian Filtered\split_indices.pth")

    # print("Split indices saved!")
        # لود اندیس‌ها
    indices = torch.load(r"E:\Datasets\Diabetic Retinopathy 224x224 Gaussian Filtered\split_indices.pth")

    train_ds = Subset(full_dataset, indices['train_indices'])
    val_ds = Subset(full_dataset, indices['val_indices'])
    test_ds = Subset(full_dataset, indices['test_indices'])

    print("Split indices loaded!")

    train_loader = DataLoader(train_ds, batch_size=batch_size, shuffle=True,
                              num_workers=num_workers, pin_memory=pin_memory)
    val_loader = DataLoader(val_ds, batch_size=batch_size, shuffle=False,
                            num_workers=num_workers, pin_memory=pin_memory)

    # --- Model ---
    model = models.efficientnet_b0(weights=models.EfficientNet_B0_Weights.IMAGENET1K_V1)
    for param in model.parameters():
        param.requires_grad = False
    num_features = model.classifier[1].in_features
    model.classifier[1] = nn.Linear(num_features, num_classes)
    for param in model.classifier.parameters():
        param.requires_grad = True
    model = model.to(device)

    criterion = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(filter(lambda p: p.requires_grad, model.parameters()), lr=learning_rate)

    # --- Training ---
    print("Starting training...")
    start_time = time.time()
    best_val_accuracy = 0.0
    patience_counter = 0
    val_accuracies = []  # List to store validation accuracies to draw plot

    for epoch in range(epochs):
        model.train()
        running_loss = 0.0
        for inputs, labels in train_loader:
            inputs, labels = inputs.to(device, non_blocking=True), labels.to(device, non_blocking=True)
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
            running_loss += loss.item()

        # Validation
        model.eval()
        correct, total = 0, 0
        with torch.no_grad():
            for inputs, labels in val_loader:
                inputs, labels = inputs.to(device), labels.to(device)
                outputs = model(inputs)
                predictions = torch.argmax(outputs, dim=1)
                total += labels.size(0)
                correct += (predictions == labels).to("cpu").sum().item()

        val_accuracy = correct / total
        val_accuracies.append(val_accuracy)  # Save val accuracy for plotting
        print(f"Epoch [{epoch+1}/{epochs}], Loss: {running_loss/len(train_loader):.4f}, "
              f"Validation Accuracy: {val_accuracy:.4f}")

        if val_accuracy > best_val_accuracy:
            best_val_accuracy = val_accuracy
            torch.save(model.state_dict(), model_path)
            print(f"✅ Saved best model to '{model_path}' with Validation Accuracy: {best_val_accuracy:.4f}")
            patience_counter = 0
        else:
            patience_counter += 1
            print(f"Validation not improved. Patience: {patience_counter}/{patience}")

        if patience_counter >= patience:
            print("⏹ Early stopping triggered.")
            break

    end_time = time.time()
    print(f"\nTraining finished in {(end_time - start_time)/60:.1f} min. Best val acc: {best_val_accuracy:.4f}")

    # After training, plot validation accuracy
    actual_epochs = len(val_accuracies) # for plotting needed to have same first dimension
    plt.figure(figsize=(8, 6))
    plt.plot(range(1, actual_epochs + 1), val_accuracies, marker='o', linestyle='-', color='b')
    plt.title('Validation Accuracy over Epochs')
    plt.xlabel('Epoch')
    plt.ylabel('Validation Accuracy')
    plt.grid(True)
    plt.show()
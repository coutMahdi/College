# test_model.py
import torch
import torch.nn as nn
from torchvision import datasets, transforms, models
from torch.utils.data import DataLoader, Subset
import os

# --- Config ---
# data_dir = r"E:\Datasets\Diabetic Retinopathy 2015 Data Colored Resized\colored_images"
# model_path = r"E:\Datasets\Diabetic Retinopathy 2015 Data Colored Resized\Models\PT_100.pt"
data_dir = r"E:\Datasets\Diabetic Retinopathy 224x224 Gaussian Filtered\gaussian_filtered_images"
model_path = r"E:\Datasets\Diabetic Retinopathy 224x224 Gaussian Filtered\Models\PT_100.pt"

if not os.path.exists(data_dir):
    print(f"Error: Dataset directory not found at '{data_dir}'")
    exit()

image_size = 224 # must be same as trained
batch_size = 16 # no need to be same: in test does not affect on acc at all. only affects performance(time and memory) bcz no grad and weight updates
num_workers = 0
pin_memory = True
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

    # استفاده از اندیس‌های ذخیره شده
    # test_indices = torch.load(r"E:\Datasets\Diabetic Retinopathy 2015 Data Colored Resized\test_indices.pth")
    test_indices = torch.load(r"E:\Datasets\Diabetic Retinopathy 224x224 Gaussian Filtered\test_indices.pth")
    test_ds = Subset(full_dataset, test_indices)

    test_loader = DataLoader(test_ds, batch_size=batch_size, shuffle=False,
                             num_workers=num_workers, pin_memory=pin_memory)

    # --- Model ---
    model = models.efficientnet_b0(weights=None)
    model.classifier[1] = nn.Linear(model.classifier[1].in_features, num_classes)
    model = model.to(device)

    try:
        model.load_state_dict(torch.load(model_path, map_location=device))
        model.eval()
        print(f"✅ Loaded trained model from '{model_path}'")

        correct, total = 0, 0
        with torch.no_grad(): 
            for inputs, labels in test_loader:
                inputs, labels = inputs.to(device), labels.to(device)
                outputs = model(inputs)
                predictions = torch.argmax(outputs, dim=1)
                total += labels.size(0)
                correct += (predictions == labels).to("cpu").sum().item()

        test_accuracy = correct / total
        print(f"📊 Test Accuracy: {test_accuracy:.4f}")

    except FileNotFoundError:
        print(f"❌ Error: Trained model not found at '{model_path}'. Run train_model.py first.")

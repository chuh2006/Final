if __name__ == '__main__':
    from ultralytics import YOLO

    model = YOLO('yolo11n.pt')

    result = model.train(data='C:/Users/33554/Documents/learn/homew/qr/data/qr/dataset.yaml', epochs=200, imgsz=640, batch=16)

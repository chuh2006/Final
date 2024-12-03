import cv2
from ultralytics import YOLO

from ultralytics.utils.plotting import Annotator, colors


def det(result, frame):
    boxes = result[0].boxes.xyxy.cpu().tolist()
    clss = result[0].boxes.cls.cpu().tolist()
    if boxes is not None:
        for box, cls in zip(boxes, clss):
            crop_obj = frame[int(box[1]) : int(box[3]), int(box[0]) : int(box[2])]
            return crop_obj

model = YOLO('last.pt')
cap = cv2.VideoCapture("C:/Users/33554/Documents/learn/homew/qr/video/20241026_231821.mp4")
while True:
    ret, frame = cap.read()
    if ret:
        result = model(frame)
        fra = det(result, frame)
        frame = result[0].plot()
        cv2.imshow("frame", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break
cap.release()
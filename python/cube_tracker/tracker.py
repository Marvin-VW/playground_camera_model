import cv2
import numpy as np

lower_pink = np.array([140, 100, 100])  # Lower bound for pink
upper_pink = np.array([170, 255, 255])  # Upper bound for pink


video_path = 'video.mp4'
cap = cv2.VideoCapture(video_path)

while cap.isOpened():
    ret, frame = cap.read()

    if not ret:
        break

    hsl_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HLS)

    mask = cv2.inRange(hsl_frame, lower_pink, upper_pink)
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    if contours:

        largest_contour = max(contours, key=cv2.contourArea)

        x, y, w, h = cv2.boundingRect(largest_contour)

        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

        center_x, center_y = x + w // 2, y + h // 2
        cv2.circle(frame, (center_x, center_y), 5, (255, 0, 0), -1)

    cv2.imshow("Pink Cube Tracker", frame)

    cv2.imshow("Mask", mask)

    # Break the loop if 'q' is pressed
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

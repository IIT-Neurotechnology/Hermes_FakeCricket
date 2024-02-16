import cv2
import multiprocessing as mp
from arrayqueues import ArrayQueue

class CameraProcess(mp.Process):
    def __init__(self, frame_queue):
        super(CameraProcess, self).__init__()
        self.frame_queue = frame_queue

    def run(self):
        # Open the camera
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("Error: Unable to access the camera.")
            return

        while True:
            ret, frame = cap.read()
            if not ret:
                break

            self.frame_queue.put(frame)

        # Release the camera and close the queue when done
        cap.release()
        self.frame_queue.put(None)

class DisplayProcess(mp.Process):
    def __init__(self, frame_queue):
        super(DisplayProcess, self).__init__()
        self.frame_queue = frame_queue

    def run(self):
        while True:
            frame = self.frame_queue.get()

            if frame is None:
                break

            cv2.imshow("Camera Feed", frame)

            # Display one frame every ten frames
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

if __name__ == "__main__":
    # Create a multiprocessing Queue to share frames between processes
    frame_queue = mp.Queue(max_mbytes=500)

    # Create and start the camera process
    camera_proc = CameraProcess(frame_queue)
    camera_proc.start()

    # Create and start the display process
    display_proc = DisplayProcess(frame_queue)
    display_proc.start()

    # Wait for both processes to finish
    camera_proc.join()
    display_proc.join()

    # Close OpenCV windows
    cv2.destroyAllWindows()
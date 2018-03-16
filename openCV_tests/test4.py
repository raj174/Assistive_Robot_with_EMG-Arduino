import cv2

cv2.namedWindow("preview")
vc = cv2.VideoCapture(1)

if vc.isOpened(): # try to get the first frame
    rval, frame = vc.read()
else:
    rval = False

while rval:

    image = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    img_blur = cv2.blur(image, (5,5))
    ret, img_bw = cv2.threshold(img_blur, 127, 255, cv2.THRESH_BINARY)
    edges = cv2.Canny(img_bw,100,200)
    contours, hierarchy = cv2.findContours(edges,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) #contours is not an image, is a chain of pixel locations
    cv2.drawContours(frame, contours,-1,(0,255,0),3)

    cv2.imshow("preview", frame)
    rval, frame = vc.read()
    key = cv2.waitKey(20)
    if key == 27: # exit on ESC
        break
cv2.destroyWindow("preview")
vc.release()


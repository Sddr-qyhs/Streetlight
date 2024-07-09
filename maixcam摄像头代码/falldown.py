from maix import camera, image, nn, time, app, http, gpio, pinmap

# 导入模型
detector = nn.YOLOv5(model="/root/models/model_132972.mud")

# 初始化摄像头
cam = camera.Camera(detector.input_width(), detector.input_height(), detector.input_format())

# 初始化 GPIO
pinmap.set_pin_function("A14", "GPIOA14")
led = gpio.GPIO("GPIOA14", gpio.Mode.OUT)
led.value(1)  # 初始状态设置为高电平

# 初始化 HTTP JPEG 流
html = """<!DOCTYPE html>
<html>
<head>
    <title>JPG Stream</title>
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #000;
        }
        .container {
            width: 90%;
            height: 90%;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        img {
            width: 100%;
            height: 100%;
            object-fit: contain;
        }
    </style>
</head>
<body>
    <div class="container">
        <img src="/stream" alt="Stream">
    </div>
</body>
</html>"""

stream = http.JpegStreamer()
stream.set_html(html)
stream.start()

print("http://{}:{}".format(stream.host(), stream.port()))

# 初始化变量
fall_detected = False
fall_start_time = 0
delay_duration = 8000  # 延时8秒

# 控制 GPIO 初始状态
led.value(1)  # 输出高电平

# 主循环
while not app.need_exit():
    t = time.ticks_ms()
    img = cam.read()
    objs = detector.detect(img, conf_th=0.5, iou_th=0.45)
    new_fall_detected = False
    for obj in objs:
        img.draw_rect(obj.x, obj.y, obj.w, obj.h, color=image.COLOR_RED)
        msg = f'{detector.labels[obj.class_id]}: {obj.score:.2f}'
        img.draw_string(obj.x, obj.y, msg, color=image.COLOR_RED)
        
        if detector.labels[obj.class_id] == "fall":
            new_fall_detected = True
            if not fall_detected:
                fall_detected = True
                fall_start_time = time.ticks_ms()  # 记录倒地开始时间

    # 控制 GPIO 输出低电平的条件
    if fall_detected and (time.ticks_ms() - fall_start_time >= delay_duration):
        led.value(0)  # 输出低电平
    else:
        led.value(1)  # 输出高电平

    # 若未检测到新的倒地事件，则重置相关变量和输出高电平
    if not new_fall_detected:
        fall_detected = False
        led.value(1)  # 输出高电平

    jpg = img.to_jpeg()
    stream.write(jpg)

    elapsed_time = time.ticks_ms() - t
    print(f"time: {elapsed_time}ms, fps: {1000 / elapsed_time}")

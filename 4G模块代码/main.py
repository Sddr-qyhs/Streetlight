import log
import checkNet
from aLiYun import aLiYun
import ujson
import _thread
import utime
from misc import ADC 
from machine import I2C
from machine import Pin
from machine import UART
utime.sleep(5)
uart2 = UART(UART.UART2, 9600, 8, 0, 1, 0)
uart0 = UART(UART.UART0, 9600, 8, 0, 1, 0)
'''
下面两个全局变量是必须有的，用户可以根据自己的实际项目修改下面两个全局变量的值
'''
PROJECT_NAME = "QuecPython_AliYin_example"
PROJECT_VERSION = "1.0.0"
checknet = checkNet.CheckNetwork(PROJECT_NAME, PROJECT_VERSION)

gpio24 = Pin(Pin.GPIO24, Pin.OUT, Pin.PULL_DISABLE, 1)
gpio23 = Pin(Pin.GPIO23, Pin.OUT, Pin.PULL_DISABLE, 1)
# 设置日志输出级别
log.basicConfig(level=log.INFO)
aliYun_log = log.getLogger("ALiYun")

productKey = "k1jrbn6Cilc"  # 产品标识(参照阿里 IoT 平台应用开发指导)
productSecret = None  # 产品密钥（使用一机一密认证时此参数传入None，参照阿里 IoT 平台应用开发指导)
DeviceName = "data"  # 设备名称(参照阿里 IoT 平台应用开发指导)
DeviceSecret = "df5ffb224b7f61b723afe2b757a71c5c"  # 设备密钥（使用一型一密认证此参数传入None，免预注册暂不支持，需先在 IoT 平台创建设备，参照阿里 IoT 平台应用开发指导)


A=3
V=4
P=5
L=2




# 回调函数
def sub_cb(topic, msg):
    global state
    aliYun_log.info("Subscribe Recv: Topic={},Msg={}".format(topic.decode(), msg.decode()))
    state = 1

    
    # 解析接收到的消息
    try:
        msg_dict = ujson.loads(msg.decode())
        params = msg_dict.get('params', {})
        alarm_switch = params.get('LightSwitch')
        
        # 判断 AlarmSwitch 是否为 1，如果是则执行对应功能
        if alarm_switch == 1:
            gpio23.write(0)#gpio23输出0
            print("灯关闭")
        if alarm_switch == 0:
            print("灯开启")
            gpio23.write(1)#gpio23输出1
            
    except ValueError:  # 如果解析JSON出错
        gpio23.write(0)
        aliYun_log.error("Failed to parse the received message as JSON.")
            
   

# 构造属性上报的消息体，遵循阿里云物联网平台规定的JSON格式
payload = {
    "id": "物联网测试设备",  # 使用当前时间戳作为消息的ID
    "version": "1.0",
    "params": {
       " LightCurrent" : A,
       "LightVolt" :V,
       "RealTimePower":P,
       "LightLux":L



    },
    "method": "thing.event.property.post",  # 属性上报的方法名
}


def publish_temperature(ali):
    """上报温度到阿里云"""
    #global a#当你在一个函数或其他局部作用域内使用global关键字声明一个变量时，你实际上是告诉Python解释器，当遇到对该变量的引用或赋值操作时，应当直接查找或修改全局作用域中的那个变量，而不是在当前作用域内创建一个新的局部变量。
    msg_json = ujson.dumps(payload)#将payload这个Python对象转换成一个JSON格式的字符
    ali.publish(topic, msg_json)#阿里云上报消息，上报topic，转换后的消息
  #  aliYun_log.info("temperature: %.2f°C", a)




if __name__ == '__main__':
    stagecode, subcode = checknet.wait_network_connected(30)
    if stagecode == 3 and subcode == 1:
        aliYun_log.info('Network connection successful!')
        # 创建aliyun连接对象
        ali = aLiYun(productKey, productSecret, DeviceName, DeviceSecret)

        # 设置mqtt连接属性
        clientID = "012345"  # 自定义字符（不超过64）
        ali.setMqtt(clientID, clean_session=False, keepAlive=300)

        # 设置回调函数
        ali.setCallback(sub_cb)
        # 物模型中属性上报的Topic模板
        topic_template = "/sys/{productKey}/{deviceName}/thing/event/property/post"

        # 实际使用的Topic，替换${productKey}和${deviceName}为实际值
        topic = topic_template.format(productKey=productKey, deviceName=DeviceName)
        # 发布消息
        # 运行
        ali.start()
        print("连接成功")
        while 1:
            gpio24.write(1)
            data = uart0.read(8)  
            L=data
            utime.sleep(1)
            print(L)

            payload["params"]["LightCurrent"] = A#更新字典中电流状态
            payload["params"]["LightVolt"] = V#更新字典中电压状态
            payload["params"]["RealTimePower"] =P#更新字典中功率状态
            payload["params"]["LightLux"] =L#更新字典中开关状态
            publish_temperature(ali)  # 每次循环都上传数据

            L=""

            utime.sleep(1)
            uart2.write(bytes("AT+V\r\n", 'utf-8'))#向电压电流测量模块发送AT指令
            utime.sleep(1)
            d=uart2.read(64)#a=串口接收的字节数据获取的电压数据
            uart_data = bytes(d, 'utf-8') #将data转utf-8编码
            print(d)
            byte_str = d
            # 解码为字符串
            str_data = byte_str.decode('utf-8')
            print(str_data)

            # 使用字符串切片或split方法提取'0.000'
            # 这里使用split方法，假设等号后直接跟着要提取的浮点数，且该数字是第一个出现的数字
            parts = str_data.split('=')
            if len(parts) > 1:
                num_str = parts[1].split('\r')[0]  # 提取等于号后面的部分，并去掉换行符
                V = float(num_str)  # 转换为float类型



            utime.sleep(2)
            uart2.write(bytes("AT+C\r\n", 'utf-8'))
            utime.sleep(1)
            c=uart2.read(64)#a=串口接收的字节数据
            uart_data = bytes(c, 'utf-8') #将data转utf-8编码
            print(c)
            byte_str = c
            # 解码为字符串
            str_data = byte_str.decode('utf-8')
            print(str_data)

            # 使用字符串切片或split方法提取'0.000'
            # 这里使用split方法，假设等号后直接跟着要提取的浮点数，且该数字是第一个出现的数字
            parts = str_data.split('=')
            if len(parts) > 1:
                num_str = parts[1].split('\r')[0]  # 提取等于号后面的部分，并去掉换行符
                A = float(num_str)  # 转换为float类型



            utime.sleep(3)
            uart2.write(bytes("AT+P\r\n", 'utf-8'))
            utime.sleep(1)
            p=uart2.read(64)#a=串口接收的字节数据
            uart_data = bytes(p, 'utf-8') #将data转utf-8编码
            print(p)
            byte_str = p
            # 解码为字符串
            str_data = byte_str.decode('utf-8')
            print(str_data)

            # 使用字符串切片或split方法提取'0.000'
            # 这里使用split方法，假设等号后直接跟着要提取的浮点数，且该数字是第一个出现的数字
            parts = str_data.split('=')
            if len(parts) > 1:
                num_str = parts[1].split('\r')[0]  # 提取等于号后面的部分，并去掉换行符
                P = float(num_str)  # 转换为float类型


           
            
          

    else:
        aliYun_log.info('Network connection failed! stagecode = {}, subcode = {}'.format(stagecode, subcode))








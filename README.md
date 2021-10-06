![](https://github.com/williamniemiec/socialine/blob/master/docs/img/logo/logo.jpg?raw=true)

<h1 align='center'>socialine</h1>
<p align='center'>News feed style notification service, similar to Twitter.</p>
<p align="center">
	<a href="https://github.com/williamniemiec/socialine/actions/workflows/macos.yml"><img src="https://github.com/williamniemiec/socialine/actions/workflows/macos.yml/badge.svg" alt=""></a>
	<a href="https://github.com/williamniemiec/socialine/actions/workflows/ubuntu.yml"><img src="https://github.com/williamniemiec/socialine/actions/workflows/ubuntu.yml/badge.svg" alt=""></a>
	<a href="https://docs.microsoft.com/en-us/cpp/"><img src="https://img.shields.io/badge/C++-17+-D0008F.svg" alt="Cpp compatibility"></a>
  <a href="https://github.com/williamniemiec/socialine/blob/master/LICENCE"><img src="https://img.shields.io/badge/Licence-BSD0-919191.svg" alt="Licence"></a>


## 🖼 Gallery

![img1](https://github.com/williamniemiec/socialine/blob/master/docs/screens/screen1.png)
![img2](https://github.com/williamniemiec/socialine/blob/master/docs/screens/screen2.png)
![img3](https://github.com/williamniemiec/socialine/blob/master/docs/screens/screen3.png)
![img4](https://github.com/williamniemiec/socialine/blob/master/docs/screens/screen4.png)
![img5](https://github.com/williamniemiec/socialine/blob/master/docs/screens/screen5.png)


## ❓ How to use

#### Client
```
cd Client
make
make run user=@<username> debug=<true_to_cli_false_to_gui>
```

#### Server
```
cd Server
make
make run
```

#### Server - debug mode
```
cd Server
make
make run debug=true
```

## ⚠ Requirements
#### wxWidgets
|        OS        |Make command|
|----------------|-------------------------------|
|Ubuntu |`make wx-ubuntu`|
|MacOS |`make wx-mac`|
|Windows| 😥 |

## 📁 Files

### /
|        Name        |Type|Description|
|----------------|-------------------------------|-----------------------------|
|Client |`Directory`|Client application|
|docs |`Directory`|Documentation files|
|Server   |`Directory`| Server application  |
|Utils   |`Directory`|Auxiliary files   |


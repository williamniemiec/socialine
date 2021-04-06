![](https://github.com/williamniemiec/socialine/blob/master/docs/img/logo/logo.jpg?raw=true)

<h1 align='center'>socialine</h1>
<p align='center'>News feed style notification service, similar to Twitter.</p>

## ❓ How to use
### Client

```
cd Client
make
make run user=@<username> server=<ip> port=<port>
```

### Server
```
cd Server
make
make run
```

## ⚠ Requirements
#### wxWidgets
```
sudo apt-add-repository universe
sudo apt-get update
sudo apt-get install libwxbase3.0-dev libwxgtk3.0-dev
```

## 📁 Files

### /
|        Name        |Type|Description|
|----------------|-------------------------------|-----------------------------|
|Client |`Directory`|Client application|
|docs |`Directory`|Documentation files|
|Server   |`Directory`| Server application  |
|Utils   |`Directory`|Auxiliary files   |


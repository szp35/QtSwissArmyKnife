﻿/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QHostAddress>
#include <QApplication>
#include <QBluetoothAddress>

#include "SAKBluetoothClientDevice.hh"
#include "SAKBluetoothClientDebugPage.hh"
#include "SAKBluetoothClientDeviceController.hh"

SAKBluetoothClientDevice::SAKBluetoothClientDevice(SAKBluetoothClientDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(debugPage, parent)
    ,mDebugPage(debugPage)
{

}

bool SAKBluetoothClientDevice::initializing(QString &errorString)
{
    QEventLoop eventLoop;
    mDeviceController = qobject_cast<SAKBluetoothClientDeviceController*>(mDebugPage->deviceController());
    auto parameters = mDeviceController->parameters().value<SAKBluetoothClientDeviceController::BluetoothClientParameters>();
    auto info = parameters.deviceInfo;

    mLowEnergyController = new QLowEnergyController(info);
    connect(mLowEnergyController, &QLowEnergyController::serviceDiscovered, this, [this](QBluetoothUuid serviceUuid){
        qDebug() << serviceUuid;
    });

    Q_UNUSED(errorString);
    return true;
}

bool SAKBluetoothClientDevice::open(QString &errorString)
{
    mLowEnergyController->connectToDevice();
    sleep(3);
    if (!(mLowEnergyController->state() == QLowEnergyController::ConnectedState)){
        return false;
    } else {
        emit messageChanged("Have been connected to device.", true);
    }

//    mLowEnergyController->discoverServices();
//    connect(mLowEnergyController, &QLowEnergyController::serviceDiscovered, this, [this](QBluetoothUuid serviceUuid){
//        qDebug() << serviceUuid;
//        auto service = mLowEnergyController->createServiceObject(serviceUuid);
//        connect(service, &QLowEnergyService::stateChanged, this, [=](){
//            qDebug() << "service state change" << mLowEnergyController->state() << ",||||||";
//            //发现服务, 建立characteristic对象实例
//            if(service->state() == QLowEnergyService::ServiceDiscovered) {
//                QLowEnergyCharacteristic hrChar = service->characteristic(QBluetoothUuid(quint16(0xfff6)));
//                if(!hrChar.isValid()) {
//                    qDebug() << "characteristic fff6 error:::";
//                }
//                // 设置特征对象可用
//                //enable the chracteristic notification by write 0x01 to client characteristic configuration
//                QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
//                if (m_notificationDesc.isValid()) {
//                    if(hrChar.properties() & QLowEnergyCharacteristic::Notify) {
//                        qDebug() << "123";
//                    }
//                    service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
//                }
//            }

//            connect(service, &QLowEnergyService::characteristicChanged, this, [](QLowEnergyCharacteristic characteristic, QByteArray value){
//                Q_UNUSED(characteristic);
//                qDebug() << value;
//            });
//        });
//    });
    Q_UNUSED(errorString);
    return mLowEnergyController->state() == QLowEnergyController::ConnectedState;
}

QByteArray SAKBluetoothClientDevice::read()
{
//    return mTcpSocket->readAll();
    return QByteArray();
}

QByteArray SAKBluetoothClientDevice::write(QByteArray bytes)
{
//    qint64 ret = mTcpSocket->write(bytes);
//    if (ret > 0){
//        return bytes;
//    }else{
//        return QByteArray();
//    }
    return QByteArray();
}

bool SAKBluetoothClientDevice::checkSomething(QString &errorString)
{
//    if(mTcpSocket->state() == QTcpSocket::UnconnectedState){
//        errorString = tr("Connection has been disconnected.");
//        return false;
//    }else{
//        errorString = tr("Unknown error");
//        return true;
//    }
    return true;
}

void SAKBluetoothClientDevice::close()
{
    emit messageChanged("Have been disconnected to device.", true);
    mLowEnergyController->disconnectFromDevice();
}

void SAKBluetoothClientDevice::free()
{
    delete mLowEnergyController;
    mLowEnergyController = Q_NULLPTR;
}

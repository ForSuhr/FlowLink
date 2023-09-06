#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QWidget>
#include <QString>
#include <QFile>

static QString qssPathLumos = ":/asset/style/lumos/lumos.qss";

class StyleSheet
{
public:
	static StyleSheet &Instance();
	void loadQSS(QWidget *widget, QString qssPath);

private:
	StyleSheet();
	~StyleSheet();
};

#endif // STYLESHEET_H

/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2022 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#include "OverridesDialog.h"
#include "../../../core/HistoryManager.h"
#include "../../../core/SettingsManager.h"

#include "ui_OverridesDialog.h"

namespace Otter
{

OverridesDialog::OverridesDialog(int identifier, QWidget *parent) : Dialog(parent),
	m_model(new QStandardItemModel(this)),
	m_identifier(identifier),
	m_ui(new Ui::OverridesDialog)
{
	m_ui->setupUi(this);

	setWindowTitle(tr("Overrides for %1").arg(SettingsManager::getOptionName(identifier)));

	const QStringList overrideHosts(SettingsManager::getOverrideHosts(identifier));

	for (int i = 0; i < overrideHosts.count(); ++i)
	{
		QList<QStandardItem*> items({new QStandardItem(HistoryManager::getIcon(overrideHosts.at(i)), overrideHosts.at(i)), new QStandardItem(SettingsManager::getOption(identifier, overrideHosts.at(i)).toString())});
		items[0]->setFlags(items[0]->flags() | Qt::ItemNeverHasChildren);
		items[1]->setFlags(items[1]->flags() | Qt::ItemNeverHasChildren);

		m_model->appendRow(items);
	}

	m_model->setHorizontalHeaderLabels({tr("Host"), tr("Value")});
	m_model->sort(0);

	m_ui->overridesViewWidget->setViewMode(ItemViewWidget::ListView);
	m_ui->overridesViewWidget->setModel(m_model);
	m_ui->overridesViewWidget->setLayoutDirection(Qt::LeftToRight);

	connect(m_ui->overridesFilterLineEditWidget, &LineEditWidget::textChanged, m_ui->overridesViewWidget, &ItemViewWidget::setFilterString);
	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &OverridesDialog::close);
}

OverridesDialog::~OverridesDialog()
{
	delete m_ui;
}

void OverridesDialog::changeEvent(QEvent *event)
{
	QDialog::changeEvent(event);

	if (event->type() == QEvent::LanguageChange)
	{
		m_ui->retranslateUi(this);

		m_model->setHorizontalHeaderLabels({tr("Host"), tr("Value")});
	}
}

}

// Copyright (c) 2011-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MYTHERRA_QT_MYTHERRAADDRESSVALIDATOR_H
#define MYTHERRA_QT_MYTHERRAADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class MytherraAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit MytherraAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Mytherra address widget validator, checks for a valid mytherra address.
 */
class MytherraAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit MytherraAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // MYTHERRA_QT_MYTHERRAADDRESSVALIDATOR_H

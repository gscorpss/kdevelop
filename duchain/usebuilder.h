/*************************************************************************************
 *  Copyright (C) 2013 by Andrea Scarpino <andrea@archlinux.org>                     *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#ifndef USEBUILDER_H
#define USEBUILDER_H

#include <language/duchain/builders/abstractusebuilder.h>

#include "contextbuilder.h"

typedef KDevelop::AbstractUseBuilder<QmlJS::AST::Node, QmlJS::AST::IdentifierPropertyName, ContextBuilder> UseBuilderBase;

class KDEVQMLJSDUCHAIN_EXPORT UseBuilder : public UseBuilderBase
{
public:
    UseBuilder();

protected:
    using Visitor::visit;
    virtual bool visit(QmlJS::AST::FunctionDeclaration* node);
    virtual bool visit(QmlJS::AST::FormalParameterList* node);
    virtual bool visit(QmlJS::AST::VariableDeclaration* node);

};

#endif // USEBUILDER_H

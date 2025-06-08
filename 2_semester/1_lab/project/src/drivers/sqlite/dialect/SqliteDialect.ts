import {Dialect} from "@/drivers/common/Dialect";
import {Query} from "@/query-builder/queries/Query";
import {SelectQuery} from "@/query-builder/queries/Select";
import {UpdateQuery} from "@/query-builder/queries/Update";
import {InsertQuery} from "@/query-builder/queries/Insert";
import {DeleteQuery} from "@/query-builder/queries/Delete";
import {ConditionClause} from "@/query-builder/queries/common/WhereClause";



export class SqliteDialect implements Dialect {
    private paramCount = 0;

    buildQuery(query: Query): { sql: string; params: any[] } {
        this.paramCount = 0;
        const params: any[] = [];

        switch (query.type) {
            case 'SELECT':
                return this.buildSelect(query, params);
            case 'INSERT':
                return this.buildInsert(query, params);
            case 'UPDATE':
                return this.buildUpdate(query, params);
            case 'DELETE':
                return this.buildDelete(query, params);
            default:
                throw new Error('Unknown query type');
        }
    }

    escapeIdentifier(str: string): string {
        return `"${str.replace(/"/g, '""')}"`;
    }

    parameterize(_index: number): string {
        return '?';
    }

    private buildSelect(query: SelectQuery, params: any[]): { sql: string; params: any[] } {
        // const parts = ['SELECT'];
        //
        // parts.push(query.fields.length > 0
        //     ? query.fields.map(f => this.escapeIdentifier(f)).join(', ')
        //     : '*');
        //
        // parts.push('FROM', this.escapeIdentifier(query.table));
        //
        // if (query.where) {
        //     const whereClause = this.buildWhere(query.where, params);
        //     if (whereClause) parts.push('WHERE', whereClause);
        // }
        //
        // if (query.orderBy) {
        //     parts.push('ORDER BY',
        //         `${this.escapeIdentifier(query.orderBy.field)} ${query.orderBy.direction}`);
        // }
        //
        // if (query.limit) {
        //     parts.push('LIMIT', this.parameterize(params.length));
        //     params.push(query.limit);
        //
        //     if (query.offset) {
        //         parts.push('OFFSET', this.parameterize(params.length));
        //         params.push(query.offset);
        //     }
        // }
        //
        // return { sql: parts.join(' '), params };

        return {sql : '', params}

    }

    private buildInsert(query: InsertQuery, params: any[]): { sql: string; params: any[] } {
        // const columns = Object.keys(query.values);
        // const values = Object.values(query.values);
        //
        // let sql = `INSERT INTO ${this.escapeIdentifier(query.table)} `;
        // sql += `(${columns.map(c => this.escapeIdentifier(c)).join(', ')}) `;
        // sql += `VALUES (${values.map(() => this.parameterize(params.length)).join(', ')})`;
        //
        // params.push(...values);
        //
        // if (query.returning && query.returning.length > 0) {
        //     sql += ` RETURNING ${query.returning
        //         .map(field => this.escapeIdentifier(field))
        //         .join(', ')}`;
        // } else {
        //     sql += '; SELECT last_insert_rowid() as id';
        // }
        //
        // return { sql, params };

        return {sql : '', params}

    }

    private buildUpdate(query: UpdateQuery, params: any[]): { sql: string; params: any[] } {
        // const sets = Object.entries(query.values).map(([key, _]) =>
        //     `${this.escapeIdentifier(key)} = ${this.parameterize(params.length)}`
        // );
        //
        // let sql = `UPDATE ${this.escapeIdentifier(query.table)} SET ${sets.join(', ')}`;
        //
        // params.push(...Object.values(query.values));
        //
        // if (query.where) {
        //     const whereClause = this.buildWhere(query.where, params);
        //     if (whereClause) sql += ` WHERE ${whereClause}`;
        // }
        //
        // if (query.returning && query.returning.length > 0) {
        //     sql += ` RETURNING ${query.returning
        //         .map(field => this.escapeIdentifier(field))
        //         .join(', ')}`;
        // }
        //
        // return { sql, params };

        return {sql : '', params}
    }

    private buildDelete(query: DeleteQuery, params: any[]): { sql: string; params: any[] } {
        // let sql = `DELETE FROM ${this.escapeIdentifier(query.table)}`;
        //
        // if (query.where) {
        //     const whereClause = this.buildWhere(query.where, params);
        //     if (whereClause) sql += ` WHERE ${whereClause}`;
        // }
        //
        // if (query.returning && query.returning.length > 0) {
        //     sql += ` RETURNING ${query.returning
        //         .map(field => this.escapeIdentifier(field))
        //         .join(', ')}`;
        // }
      //
        // return { sql, params };

        return {sql : '', params}
    }      
       
    private buildWhere(where: ConditionClause, params: any[]): string {
        // const conditions = Object.entries(where).map(([key, value]) => {
        //     if (value === null) {
        //         return `${this.escapeIdentifier(key)} IS NULL`;
        //     }
        //     if (Array.isArray(value)) {
        //         params.push(...value);
        //         return `${this.escapeIdentifier(key)} IN (${value
        //             .map(() => this.parameterize(params.length))
        //             .join(', ')})`;
        //     }
        //     params.push(value);
        //     return `${this.escapeIdentifier(key)} = ${this.parameterize(params.length - 1)}`;
        // });
        //
        // return conditions.join(' AND ');

        return ''
    }
}